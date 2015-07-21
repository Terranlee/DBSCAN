
#ifndef HWDOUBLE_SPECIALIZATION_H_
#define HWDOUBLE_SPECIALIZATION_H_


/* This header provides a specialized version of HWFloat.
 *
 * Type provided: HWDouble (aka. HWFloat<11,53>)
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
#define DOUBLE_EXPONENT_SELECT_MASK 0x7FF0000000000000l
#define DOUBLE_MANTISSA_SELECT_MASK 0x000FFFFFFFFFFFFFl
#define DOUBLE_EXPONENT_SHIFT 52
#define DOUBLE_SIGN_SHIFT 63
#define DOUBLE_EXPONENT_SIZE 11
#define DOUBLE_EXPONENT_BITS DOUBLE_EXPONENT_SIZE
#define DOUBLE_MANTISSA_SIZE 53
#define DOUBLE_MANTISSA_BITS DOUBLE_EXPONENT_SHIFT


#define EX_TPL template<bool hw_exception>

/* HWFloat specialization that matches single precision */
typedef HWFloat<DOUBLE_EXPONENT_SIZE,DOUBLE_MANTISSA_SIZE> HWDouble;

template <> class HWFloat<DOUBLE_EXPONENT_SIZE,DOUBLE_MANTISSA_SIZE>
{
private:
	double m_value;

	typedef union {
		double float_value;
		uint64_t int_value;
	} double_cast_union;

	// == CONST METHODS ==
	inline bool getSign() const
	{
		return m_value < 0.0;
	}
	inline varint_u<DOUBLE_EXPONENT_SIZE> getExponent() const
	{
		int exponent = ((uint64_t&)m_value & EXPONENT_SELECT_MASK) >> DOUBLE_EXPONENT_SHIFT;
		return varint_u<DOUBLE_EXPONENT_SIZE>( exponent );
	}
	inline varint_u<DOUBLE_MANTISSA_BITS> getMantissa() const
	{
		int mantissa = ((uint64_t&)m_value & DOUBLE_MANTISSA_SELECT_MASK);
		return varint_u<DOUBLE_MANTISSA_BITS>( mantissa );
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
	HWFloat(bool sign, varint_u<DOUBLE_EXPONENT_BITS> exponent, varint_u<DOUBLE_MANTISSA_BITS> mantissa)
	{
		double_cast_union buffer;
		buffer.int_value = (( (uint64_t)sign & 0x1)  << DOUBLE_SIGN_SHIFT)
				           |  (uint64_t)exponent     << DOUBLE_MANTISSA_BITS
				           | ((uint64_t)mantissa & MANTISSA_SELECT_MASK);
		m_value = buffer.float_value;
	}

	// == STATIC ==
	static HWDouble NaN()
	{
		return HWDouble(NAN);
	}
	static HWDouble Infty(bool sign)
	{
		return sign ? HWDouble(-INFINITY) : HWDouble(INFINITY);
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
	enum { totalBitSize = 64 };

	// == CONST METHODS ==
	varint_u<64> getBitString() const
	{
		double_cast_union buffer;
		buffer.float_value = m_value;
		varint_u<64> result = buffer.int_value;
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
		double_cast_union buffer;
		buffer.float_value = m_value;
		return (buffer.int_value & 0x7FFFFFFFFFFFFFFF) != 0;
	}
	std::string getTypeString() const
	{
		return "{HWDouble_native}";
	}
	// Arithmetic Operations
	inline HWDouble  neg_float () const
	{
		return HWDouble(-m_value);
	}
	EX_TPL inline HWDouble  add_float (const HWDouble &summand, EXCEPTFLOAT = NULL) const;
	EX_TPL inline HWDouble  sub_float (const HWDouble &subtrahend, EXCEPTFLOAT = NULL) const;
	EX_TPL inline HWDouble  mul_float (const HWDouble &multiplicand, EXCEPTFLOAT = NULL) const;
	EX_TPL inline HWDouble  div_float (const HWDouble &dividend, EXCEPTFLOAT = NULL) const;
	EX_TPL inline HWDouble sqrt_float (EXCEPTFLOAT = NULL) const;
	// Comparison
	inline HWBool  eq_float (const HWDouble &b) const
	{
		return HWBool( m_value == b.m_value );
	}
	inline HWBool neq_float (const HWDouble &b) const
	{
		return HWBool( m_value != b.m_value );
	}
	inline HWBool  gt_float (const HWDouble &b) const
	{
		return HWBool( m_value >  b.m_value );
	}
	inline HWBool gte_float (const HWDouble &b) const
	{
		return HWBool( m_value >= b.m_value );
	}

	// == MODIFIERS ==
	// Constructors
	HWFloat(varint_u<64> raw_bits)
	{
		double_cast_union buffer;
		buffer.int_value = (uint64_t) raw_bits;
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

	friend std::ostream& operator<< (std::ostream &o, const HWDouble &v)
	{
		if(o.flags() & std::ios_base::dec)
			return o << v.getValueAsDouble();
		else
			return o << v.getBitString();
	}
};

/* == Wrapper functions, with and without HWExceptions == */

// Arithmetic
inline HWDouble neg_float(const HWDouble &a) {return a.neg_float();}

inline HWDouble add_float(const HWDouble &a, const HWDouble &b) {return a.add_float<false>(b);}
inline HWDouble add_float(const HWDouble &a, const HWDouble &b, EXCEPTFLOAT) {return a.add_float<true>(b, exception);}

inline HWDouble sub_float(const HWDouble &minuend, const HWDouble &subtrahend) {return minuend.sub_float<false>(subtrahend);}
inline HWDouble sub_float(const HWDouble &minuend, const HWDouble &subtrahend, EXCEPTFLOAT) {return minuend.sub_float<true>(subtrahend, exception);}

inline HWDouble mul_float(const HWDouble &factor_a, const HWDouble &factor_b) {return factor_a.mul_float<false>(factor_b);}
inline HWDouble mul_float(const HWDouble &factor_a, const HWDouble &factor_b, EXCEPTFLOAT) {return factor_a.mul_float<true>(factor_b, exception);}

inline HWDouble div_float(const HWDouble &dividend, const HWDouble &divisor) {return dividend.div_float<false>(divisor);}
inline HWDouble div_float(const HWDouble &dividend, const HWDouble &divisor, EXCEPTFLOAT) {return dividend.div_float<false>(divisor, exception);}

inline HWDouble sqrt_float(const HWDouble &a) {return a.sqrt_float<false>();}
inline HWDouble sqrt_float(const HWDouble &a, EXCEPTFLOAT) {return a.sqrt_float<true>(exception);}

// Comparison
inline HWBool  eq_float (const HWDouble &a, const HWDouble &b) {return a.eq_float(b);}
inline HWBool neq_float (const HWDouble &a, const HWDouble &b) {return a.neq_float(b);}
inline HWBool  gt_float (const HWDouble &a, const HWDouble &b) {return a.gt_float(b);}
inline HWBool gte_float (const HWDouble &a, const HWDouble &b) {return a.gte_float(b);}
inline HWBool  lt_float (const HWDouble &a, const HWDouble &b) {return b.gt_float(a);}
inline HWBool lte_float (const HWDouble &a, const HWDouble &b) {return b.gte_float(a);}

// Casting -- done with the original/non-specialized templates. (Via accessing private members...)
} /* end namespace maxcompilersim */

#endif /* HWDOUBLE_SPECIALIZATION_H_ */
