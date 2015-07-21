#ifndef HWFLOAT_DECL_H_
#define HWFLOAT_DECL_H_

/**
 * HWFloat, arbitrary precision floating point implementation that
 * is compliant with the behavior of CoreGen's floating point core.
 * This implies that HWFloat doesn't support denormalized numbers
 * which aren't included in CoreGen either.
 *
 * For implementation hints on floating point see
 *
 * J.T. Coonen: An Implementation Guide to a Proposed Standard for
 * Floating-Point Arithmetic. DOI:10.1109/MC.1980.1653344
 *
 * http://www.computer.org/portal/web/csdl/doi/10.1109/MC.1980.1653344
 */

#include <sstream>
#include <iostream>

#include "HWTypes.h"

namespace maxcompilersim {

CLASS_HWFloat {
public:
	enum { totalBitSize = esize+msize };

	// Constructors
	HWFloat(varint_u<esize+msize> raw_bits);
	HWFloat(double v);
	HWFloat(long v);
	HWFloat(int v) { *this = HWFloat(long(v)); }
	HWFloat();

	// Getters
	varint_u<esize+msize> getBitString() const;
	double getValueAsDouble() const;
	long getValueAsLong() const;
	bool getValueAsBool() const;

	friend std::ostream& operator<<(std::ostream& o, const HWFloat<esize,msize>& v) {
		if(o.flags() & std::ios_base::dec)
			return o << v.getValueAsDouble();
		else
			return o << v.getBitString();
	}

	// Report type
	std::string getTypeString() const {
		std::ostringstream tmp;
		tmp << "{HWFloat:" << esize << ", " << msize << "}";
		return tmp.str();
	}

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

	// Casting
	CAST_float2float(friend,HWEXCEPT,EXCEPTFLOAT);
	CAST_float2fixed(friend,HWEXCEPT,EXCEPTFLOAT);
	CAST_fixed2float(friend,HWEXCEPT,EXCEPTFLOAT);

private:
	// Helpers
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


	static const uint s_ebits = esize;
	static const uint s_mbits = msize - 1;

	bool m_sgn;
	varint_u<s_ebits> m_exp;
	varint_u<s_mbits> m_mnt;

	HWFloat(bool sign, varint_u<s_ebits> exponent, varint_u<s_mbits> mantissa)
		: m_sgn(sign)
		, m_exp(exponent)
		, m_mnt(mantissa)
		{ }

	bool getSign() const { return m_sgn; }
	varint_u<s_ebits> getExponent() const { return m_exp; }
	varint_u<s_mbits> getMantissa() const { return m_mnt; }

	bool isNanInfinity() const {
		varint_u<s_ebits> exp_nan_infinity = 0;
		exp_nan_infinity = ~exp_nan_infinity;
		return (m_exp == exp_nan_infinity);
	}
	bool isNan() const { return isNanInfinity() && m_mnt; }
	bool isInfinity() const { return isNanInfinity() && (! m_mnt); }
	bool isZero() const { return !m_exp && !m_mnt; }
	bool isZeroOrDenormal() const { return !m_exp; }

	static HWFloat<esize, msize> NaN() {
		// CoreGen always produces positive 'normalized NaNs'
		varint_u<esize> exponent = 0;
		exponent = ~exponent;

		varint_u<msize-1> mantissa = 0;
		mantissa[msize-2] = true;

		return HWFloat<esize, msize>(false, exponent, mantissa);
	}

	static HWFloat<esize, msize> Infty(bool sign) {
		varint_u<esize> exponent = 0;
		exponent = ~exponent;

		varint_u<msize-1> mantissa = 0;

		return HWFloat<esize, msize>(sign, exponent, mantissa);
	}
};

} /* end namespace maxcompilersim */

#endif /* HWFLOAT_DECL_H_ */
