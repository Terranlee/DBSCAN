
#ifndef HWSINGLE_SPECIALIZATION_H_
#define HWSINGLE_SPECIALIZATION_H_


/* This header provides a specialized version of HWFloat.
 *
 * Type provided: HWSingle (aka. HWFloat<8,24>)
 *
 * All (un-templated) methods have been implemented inside the
 * declaration, hence no separate .cpp file is needed during
 * compilation. However, doing so slows down compilation as for
 * each .cpp file that includes this header, gcc must compile
 * all the method implementations again. Hence, the use of
 * pre-compiled headers is highly recommended.
 *
 * General notes on the implementation:
 *    - All operations have been specialized EXCEPT for casts.
 */


#include <sstream>
#include <iostream>

#include "HWTypes.h"
// fpclassify is normally found in math.h; However,
// after including <boost/thread/condition_variable.hpp>
// the compiler seems to have forgotten fpclassify -
// hence we include the boost version here..
// welcome to c/c++ inclusion file hell...
#include <boost/math/special_functions/fpclassify.hpp>
#include <fenv.h>

namespace maxcompilersim {

#define ISINFINITE(x) (boost::math::fpclassify (x) == FP_INFINITE)
#define ISNAN(x) isnan(x)
#define ISZERO(x) (boost::math::fpclassify (x) == FP_ZERO)
#define EXPONENT_SELECT_MASK 0x7F800000
#define MANTISSA_SELECT_MASK 0x007FFFFF

#define EX_TPL template<bool hw_exception>

/* HWFloat specialization that matches single precision */
typedef HWFloat<8,24> HWSingle;

template <> class HWFloat<8,24>
{
private:
	float m_value;

	typedef union {
		float float_value;
		uint32_t int_value;
	} float_cast_union;

	// == CONST METHODS ==
	inline bool getSign() const
	{
		return m_value < 0.0;
	}
	inline varint_u<8> getExponent() const
	{
		int exponent = ((int&)m_value & EXPONENT_SELECT_MASK) >> 23;
		return varint_u<8>( exponent );
	}
	inline varint_u<23> getMantissa() const
	{
		int mantissa = ((int&)m_value & MANTISSA_SELECT_MASK);
		return varint_u<23>( mantissa );
	}
	inline bool isNanInfinity() const
	{
		return ISINFINITE(m_value) || ISNAN(m_value);
	}
	inline bool isNan() const
	{
		return ISNAN(m_value);
	}
	inline bool isInfinity() const
	{
		return ISINFINITE(m_value);
	}
	inline bool isZero() const
	{
		return ISZERO(m_value);
	}

	// == MODIFIERS ==
	// Private Constructor
	HWFloat(bool sign, varint_u<8> exponent, varint_u<23> mantissa)
	{
		float_cast_union buffer;
		buffer.int_value = (((int)sign & 0x1) << 31) | (int)exponent << 23 | ((int)mantissa & MANTISSA_SELECT_MASK);
		m_value = buffer.float_value;
	}

	// == STATIC ==
	static HWSingle NaN()
	{
		return HWSingle(NAN);
	}
	static HWSingle Infty(bool sign)
	{
		return sign ? HWSingle(-INFINITY) : HWSingle(INFINITY);
	}

	// == FRIENDS (original code) ==
	template <uint r_esize, uint r_msize HWEXCEPT>
	friend HWFloat<r_esize, r_msize> normalize_float(
		bool force_negative,
		varint_u<r_esize> exponent,
		varint_s<r_msize+3> mantissa,
		bool guard_bit,
		bool round_bit,
		bool sticky_bit,
		EXCEPTFLOAT);

	template <uint r_esize, uint r_msize, uint a_esize, uint a_msize HWEXCEPT>
	friend HWFloat<r_esize, r_msize> _cast_float2float(
		const HWFloat<a_esize, a_msize> &a, EXCEPTFLOAT,
		Int2Type<true> real_cast);

public:
	enum { totalBitSize = 32 };

	// == CONST METHODS ==
	varint_u<32> getBitString() const
	{
		float_cast_union buffer;
		buffer.float_value = m_value;
		varint_u<32> result = buffer.int_value;
		return result;
	}
	double  getValueAsDouble() const
	{
		return m_value;
	}
	long    getValueAsLong() const
	{
		return (long)m_value;
	}
	bool    getValueAsBool() const
	{
		float_cast_union buffer;
		buffer.float_value = m_value;
		return (buffer.int_value & 0x7FFFFFFF) != 0;
	}
	std::string getTypeString() const
	{
		return "{HWFloat_native: 8, 24}";
	}
	// Arithmetic Operations
	inline HWSingle  neg_float () const
	{
		return HWFloat<8,24>(-m_value);
	}
	EX_TPL inline HWSingle  add_float (const HWSingle &summand, EXCEPTFLOAT = NULL) const;
	EX_TPL inline HWSingle  sub_float (const HWSingle &subtrahend, EXCEPTFLOAT = NULL) const;
	EX_TPL inline HWSingle  mul_float (const HWSingle &multiplicand, EXCEPTFLOAT = NULL) const;
	EX_TPL inline HWSingle  div_float (const HWSingle &dividend, EXCEPTFLOAT = NULL) const;
	EX_TPL inline HWSingle sqrt_float (EXCEPTFLOAT = NULL) const;
	// Comparison
	inline HWBool  eq_float (const HWSingle &b) const
	{
		return HWBool( m_value == b.m_value );
	}
	inline HWBool neq_float (const HWSingle &b) const
	{
		return HWBool( m_value != b.m_value );
	}
	inline HWBool  gt_float (const HWSingle &b) const
	{
		return HWBool( m_value >  b.m_value );
	}
	inline HWBool gte_float (const HWSingle &b) const
	{
		return HWBool( m_value >= b.m_value );
	}

	// == MODIFIERS ==
	// Constructors
	HWFloat(varint_u<32> raw_bits)
	{
		float_cast_union buffer;
		buffer.int_value = (unsigned int) raw_bits;
		m_value = buffer.float_value;
	}
	HWFloat(double v) : m_value(v) {};
	HWFloat(float v): m_value(v) {};
	HWFloat(long v) : m_value(v) {};
	HWFloat(int v) { *this = HWFloat(long(v)); }
	HWFloat(){};

	// == FRIENDS ==

	// Operators
	ADD_float(friend,HWEXCEPT,EXCEPTFLOAT);
	SUB_float(friend,HWEXCEPT,EXCEPTFLOAT);
	MUL_float(friend,HWEXCEPT,EXCEPTFLOAT);
	DIV_float(friend,HWEXCEPT,EXCEPTFLOAT);
	NEG_float(friend);
	SQRT_float(friend,HWEXCEPT,EXCEPTFLOAT);

	// Comparison
	EQ_float(friend);
	NEQ_float(friend);
	GT_float(friend);
	GTE_float(friend);
	LT_float(friend);
	LTE_float(friend);


	// Casting (original, non-specialized code)
	CAST_float2float(friend,HWEXCEPT,EXCEPTFLOAT);
	CAST_float2fixed(friend,HWEXCEPT,EXCEPTFLOAT);
	CAST_fixed2float(friend,HWEXCEPT,EXCEPTFLOAT);

	friend std::ostream& operator<< (std::ostream &o, const HWSingle &v)
	{
		if(o.flags() & std::ios_base::dec)
			return o << v.getValueAsDouble();
		else
			return o << v.getBitString();
	}
};

/* == Wrapper functions, with and without HWExceptions == */

// Arithmetic
inline HWSingle neg_float(const HWSingle &a) {return a.neg_float();}

inline HWSingle add_float(const HWSingle &a, const HWSingle &b) {return a.add_float<false>(b);}
inline HWSingle add_float(const HWSingle &a, const HWSingle &b, EXCEPTFLOAT) {return a.add_float<true>(b, exception);}

inline HWSingle sub_float(const HWSingle &minuend, const HWSingle &subtrahend) {return minuend.sub_float<false>(subtrahend);}
inline HWSingle sub_float(const HWSingle &minuend, const HWSingle &subtrahend, EXCEPTFLOAT) {return minuend.sub_float<true>(subtrahend, exception);}

inline HWSingle mul_float(const HWSingle &factor_a, const HWSingle &factor_b) {return factor_a.mul_float<false>(factor_b);}
inline HWSingle mul_float(const HWSingle &factor_a, const HWSingle &factor_b, EXCEPTFLOAT) {return factor_a.mul_float<true>(factor_b, exception);}

inline HWSingle div_float(const HWSingle &dividend, const HWSingle &divisor) {return dividend.div_float<false>(divisor);}
inline HWSingle div_float(const HWSingle &dividend, const HWSingle &divisor, EXCEPTFLOAT) {return dividend.div_float<false>(divisor, exception);}

inline HWSingle sqrt_float(const HWSingle &a) {return a.sqrt_float<false>();}
inline HWSingle sqrt_float(const HWSingle &a, EXCEPTFLOAT) {return a.sqrt_float<true>(exception);}

// Comparison
inline HWBool  eq_float (const HWSingle &a, const HWSingle &b) {return a.eq_float(b);}
inline HWBool neq_float (const HWSingle &a, const HWSingle &b) {return a.neq_float(b);}
inline HWBool  gt_float (const HWSingle &a, const HWSingle &b) {return a.gt_float(b);}
inline HWBool gte_float (const HWSingle &a, const HWSingle &b) {return a.gte_float(b);}
inline HWBool  lt_float (const HWSingle &a, const HWSingle &b) {return b.gt_float(a);}
inline HWBool lte_float (const HWSingle &a, const HWSingle &b) {return b.gte_float(a);}

// Casting -- done with the original/non-specialized templates. (Via accessing private members...)
} /* end namespace maxcompilersim */

#endif /* HWSINGLE_SPECIALIZATION_H_ */
