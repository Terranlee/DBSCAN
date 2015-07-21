#ifndef HWFLOAT_IMPL_H_
#define HWFLOAT_IMPL_H_

#include <math.h>
#include <fenv.h>

#include "HWFloat_decl.h"

#define MAX(a, b) ( ((a)>(b)) ? (a) : (b) )
#define MIN(a, b) ( ((a)>(b)) ? (b) : (a) )

namespace maxcompilersim {

// Constructors //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <uint esize, uint msize> inline
HWFloat<esize,msize>::HWFloat(varint_u<esize+msize> raw_bits)
	: m_sgn(raw_bits[esize+msize-1])
	, m_exp(raw_bits >> s_mbits)
	, m_mnt(raw_bits)
	{ }

template <uint esize, uint msize> inline
HWFloat<esize,msize>::HWFloat(double v) {
	DoubleBits convert;
	convert.d = v;

	HWFloat<11,53> dblFloat( varint_u<64>(convert.b) );
	HWFloat<esize,msize> trueFloat = cast_float2float<esize,msize>(dblFloat);
	*this = trueFloat;
}

template <uint esize, uint msize> inline
HWFloat<esize,msize>::HWFloat(long v) {
	HWOffsetFix<64, 0, TWOSCOMPLEMENT> fix_long = v;
	HWFloat<esize, msize> trueFloat = cast_fixed2float<esize, msize>(fix_long);
	*this = trueFloat;
}

template <uint esize, uint msize> inline
HWFloat<esize,msize>::HWFloat()
	// TODO Uninitialized value
	{ }

// Getters ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <uint esize, uint msize> inline
varint_u<esize+msize> HWFloat<esize,msize>::getBitString() const {
	varint_u<esize+msize> result = m_mnt;
	result |= (varint_u<esize+msize>(m_exp)) << s_mbits;
	if (m_sgn)
		result[esize+msize-1] = true;
	return result;
}

template <uint esize, uint msize> inline
double HWFloat<esize,msize>::getValueAsDouble() const {
	HWFloat<11,53> dblFloat = cast_float2float<11,53>(*this);
	varint_u<64>   dblBits  = dblFloat.getBitString();

	DoubleBits convert;
	convert.b = dblBits.get_bits(0,64);
	return convert.d;
}

template <uint esize, uint msize> inline
long HWFloat<esize,msize>::getValueAsLong() const {
	HWOffsetFix<64,0,TWOSCOMPLEMENT> val_long = cast_float2fixed<64,0,TWOSCOMPLEMENT>(*this);
	return val_long.getValueAsLong();
}

template <uint esize, uint msize> inline
bool HWFloat<esize,msize>::getValueAsBool() const {
	return !isZero();
}

// Helpers ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Normalize a floating point number, aligning the 1,... properly and performing round-to-
 * nearest-even rounding. This method can handle 0 and infinity numbers, but shouldn't be
 * called when you want to have a NaN.
 */
template <uint r_esize, uint r_msize HWEXCEPT>
HWFloat<r_esize, r_msize> normalize_float(
	bool force_negative,						/// Always return a negative float. If false, the sign is deduced from mantissa
	varint_u<r_esize> exponent,					/// The exponent for the supplied (un-normalized) mantissa
	varint_s<r_msize+3> mantissa,				/// The mantissa in two's complement, as {Sgn}{Int2..0}{Fractional:-1..-(msize-1)}
	bool guard_bit,								/// The guard bit, i.e. {Fractional:-msize}
	bool round_bit,								/// The rounding bit, i.e. {Fractional:-(msize+1)}
	bool sticky_bit,							/// The sticky bit, i.e. OR({Fractional:-(msize+2)..-Infinity}
	EXCEPTFLOAT)
{
	// Preset the exception to 0 in case we return in one of the special cases
	if (hw_exception)
		*exception = ExceptFloat(0);

	// Special case: The exponent is NaN or Infinity ~> our number is Infinity
	// (if it was NaN you shouldn't have used this method!)
	varint_u<r_esize> exp_infinity = 0; --exp_infinity;
	if (exponent == exp_infinity) {
		return HWFloat<r_esize, r_msize>(force_negative, exp_infinity, varint_u<r_msize-1>(0));
	}

	// Special case: The mantissa is 0 ~> our number is zero
	if (!mantissa) {
		// Will rounding un-zero the mantissa?
		if (guard_bit) {
			if(round_bit || sticky_bit) {
				mantissa[0] = true;
				guard_bit   = false;
				round_bit   = false;
				sticky_bit  = false;
			}
			else if(exponent > varint_u<r_esize>(r_msize))
				return HWFloat<r_esize, r_msize>(force_negative, exponent-varint_u<r_esize>(r_msize), varint_u<r_msize-1>(0));
			else
				return HWFloat<r_esize, r_msize>(force_negative, varint_u<r_esize>(0), varint_u<r_msize-1>(0));
		}
		// Or will the result be a 'real' zero?
		else {
			return HWFloat<r_esize, r_msize>(force_negative, varint_u<r_esize>(0), varint_u<r_msize-1>(0));
		}
	}

	// Check the signed'ness of mantissa
	if (mantissa[r_msize+2]) {					// more efficient than "if (mantissa < 0)"
		force_negative = true;

		// Treat the round bits as part of the number for 2's complement
		if (sticky_bit) {						// One's complement, no overflow into number or round/guard bit
			mantissa = ~mantissa;
			guard_bit = !guard_bit;
			round_bit = !round_bit;
		}
		else if (round_bit)	{					// Overflow into round bit, number/guard bit: one's complement
			mantissa = ~mantissa;
			guard_bit = !guard_bit;
		}
		else if (guard_bit)						// Overflow into guard bit, number: one's complement
			mantissa = ~mantissa;
		else 									// Overflow into number: two's complement
			mantissa = -mantissa;
	}

	// Find the leading-1 in mantissa
	int leading_one = r_msize+1;
	while (mantissa[leading_one] == false && leading_one >= 0)
		--leading_one;

	// Shift to obtain normalized form
	int shift_right = leading_one - (r_msize-1);
	if (shift_right == 2) {						// Maximum possible right shift
		sticky_bit |= (guard_bit | round_bit);
		round_bit = mantissa[0];
		guard_bit = mantissa[1];
		mantissa >>= 2;
	}
	else if (shift_right == 1) {
		sticky_bit |= round_bit;
		round_bit = guard_bit;
		guard_bit = mantissa[0];
		mantissa >>= 1;
	}
	else if (shift_right == -1) {	// Left shift by one, enter guard bit, keep round info
		mantissa <<= 1;
		mantissa[0] = guard_bit;
		guard_bit = round_bit;
		round_bit = false;
	}
	else if (shift_right < -1) {	// Left shift by more, just use guard bit, discard round info
		mantissa <<= -shift_right;
		mantissa[-shift_right-1] = guard_bit;
		guard_bit = false;
		round_bit = false;
		sticky_bit = false;
	}

	// Perform Round-to-nearest-even
	if (guard_bit && (mantissa[0] || sticky_bit || round_bit)) {
		++mantissa;

		// Check if rounding de-aligned the mantissa again (this can only
		// ever be the case for an mantissa of all 1s
		if (mantissa[r_msize]) {
			mantissa >>= 1;
			shift_right++;
		}
	}

	// The mantissa is fully aligned now, let's work out the exponent
	if (shift_right == 0)
		; // Nothing to do?
	else if (shift_right > 0) {
		varint_u<r_esize> exponent_old = exponent;
		exponent += shift_right;

		// Check for overflow
		if (! (exponent > exponent_old))
			exponent = exp_infinity;

		// Check for infinity (mind, we might have hit it on the spot, without overflowing!)
		if (exponent == exp_infinity) {
			if (hw_exception)
				*exception = ExceptFloat(2);
			return HWFloat<r_esize, r_msize>(force_negative, exp_infinity, varint_u<r_msize-1>(0));
		}
	}
	else { // (shift_right < 0)
		varint_u<r_esize> exponent_old = exponent;
		exponent += shift_right;

		// Check for underflow
		if (! (exponent < exponent_old))
			exponent = 0;

		// Check for zero exponent, which is always an underflow when shifting to the left (because we don't do denormals)
		if (hw_exception)
			if (! exponent)
				*exception = ExceptFloat(1);
	}

	if (!exponent) {
		if(hw_exception)
			if(mantissa)
				*exception = ExceptFloat(1);
		return HWFloat<r_esize, r_msize>(force_negative, varint_u<r_esize>(0), varint_u<r_msize-1>(0));
	}
	else
		return HWFloat<r_esize, r_msize>(force_negative, exponent, varint_u<r_msize-1>(mantissa));
}

// Casting ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
 * We define two internal casting methods _cast_float2float as in HWOffsetFix.
 */

template <
	uint r_esize, uint r_msize,
	uint a_esize, uint a_msize
	HWEXCEPT
> inline
HWFloat<r_esize, r_msize>
_cast_float2float(const HWFloat<a_esize, a_msize> &a, EXCEPTFLOAT, Int2Type<false> real_cast) {
	if (hw_exception)
		*exception = ExceptFloat(0);

	return a;
}

template <
	uint r_esize, uint r_msize,
	uint a_esize, uint a_msize
	HWEXCEPT
> inline
HWFloat<r_esize, r_msize>
_cast_float2float(const HWFloat<a_esize, a_msize> &a, EXCEPTFLOAT, Int2Type<true> real_cast) {
	// Preset the exception to 0 in case we return in one of the special cases
	if (hw_exception)
		*exception = ExceptFloat(0);

	// Special Case: a = Zero
	if (a.isZero())
		return HWFloat<r_esize, r_msize>(a.getSign(), varint_u<r_esize>(0), varint_u<r_msize-1>(0));

	// Special Case: a = NaN or Infinity
	varint_u<r_esize> exponent = 0;
	exponent = ~exponent;

	if (a.isInfinity())
		return HWFloat<r_esize, r_msize>(a.getSign(), exponent, varint_u<r_msize-1>(0));

	if (a.isNan())
		return HWFloat<r_esize, r_msize>::NaN();

	// Resize the exponent, differentiate between growing and shrinking exponent,
	// check for over/underflow when the exponent is shrinking
	if (r_esize == a_esize) {
		exponent = a.getExponent();
	}
	else if (r_esize > a_esize) {
		exponent = a.getExponent();

		// Un-bias
		varint_s<r_esize> exp_bias;
		exp_bias[a_esize-1] = true;
		--exp_bias;
		exponent -= exp_bias;

		// Re-bias
		exp_bias = 0;
		exp_bias[r_esize-1] = true;
		--exp_bias;
		exponent += exp_bias;
	}
	else { // (r_esize < a_esize)
		// We need to add one bit for the sign, otherwise our overflow/underflow comparisons might go wrong
		varint_s<a_esize+1> exp_signed = a.getExponent();

		// Un-bias
		varint_s<a_esize+1> exp_bias;
		exp_bias[a_esize-1] = true;
		--exp_bias;
		exp_signed -= exp_bias;

		// Re-bias
		exp_bias = 0;
		exp_bias[r_esize-1] = true;
		--exp_bias;
		exp_signed += exp_bias;

		// Check for underflow
		if (exp_signed <= varint_s<a_esize+1>(0)) {
			if (hw_exception)
				*exception = ExceptFloat(1);
			return HWFloat<r_esize, r_msize>(a.getSign(), varint_u<r_esize>(0), varint_u<r_msize-1>(0));
		}

		// Check for overflow
		varint_s<a_esize+1> exp_infinity;		// NaN/Infinity exponent for target size
		exp_infinity[r_esize] = true;
		--exp_infinity;
		if (exp_signed >= exp_infinity) {
			if (hw_exception)
				*exception = ExceptFloat(2);
			return HWFloat<r_esize, r_msize>(a.getSign(), exponent, varint_u<r_msize-1>(0));
		}

		exponent = exp_signed;
	}


	// Resize the mantissa, for shrinking mantissa, work out the rounding bits and send the number
	// off to the normalizer.
	if (r_msize >= a_msize) {
		varint_u<r_msize-1> mantissa;
		mantissa = a.getMantissa();
		mantissa <<= (r_msize-a_msize);
		// if (hw_exception)
		//	;	// *exception = 0 is already preset from above!
		return HWFloat<r_esize, r_msize>(a.getSign(), exponent, mantissa);
	}
	else { // (r_msize < a_msize)
		varint_s<r_msize+3> mantissa;

		mantissa = a.getMantissa() >> (a_msize-r_msize);
		mantissa[r_msize-1] = true;						// Explicit '1,'

		// Check if rounding is required
		bool guard_bit  = (int(a_msize-r_msize-1) >= 0) ? (a.getMantissa())[a_msize-r_msize-1] : false;
		bool round_bit  = (int(a_msize-r_msize-2) >= 0) ? (a.getMantissa())[a_msize-r_msize-2] : false;
		bool sticky_bit = (int(a_msize-r_msize-3) >= 0) ? (bool)(a.getMantissa() << (r_msize+1)) : false;

		return normalize_float<r_esize, r_msize, hw_exception>(a.getSign(), exponent, mantissa, guard_bit, round_bit, sticky_bit, exception);
	}
}

CAST_float2float(inline,HWEXCEPT,EXCEPTFLOAT) {
	const bool real_cast =
			r_esize != a_esize ||
			r_msize != a_msize;

	return _cast_float2float<
		r_esize, r_msize,
		a_esize, a_msize,
		hw_exception
	>(a, exception, Int2Type<real_cast>());
}


template<int sz, int off>
struct _FindMaxExponent {
	enum {
        exp_unbiased_abs = (((sz-1+off) > 0) ? (sz-1+off) : -(sz-1+off)),
        exp_maxshift_abs = ((off > 0) ? off : -off),
		exp_maxabs = (exp_maxshift_abs > exp_unbiased_abs) ? exp_maxshift_abs : exp_unbiased_abs
//        exp_maxabs = ( ((off > 0) ? off : -off) > (((sz-1+off) > 0) ? (sz-1+off) : -(sz-1+off)) )
//                        ? ((off > 0) ? off : -off) : (((sz-1+off) > 0) ? (sz-1+off) : -(sz-1+off))
	};
};

CAST_fixed2float(,HWEXCEPT,EXCEPTFLOAT) {
	// Conversion idea: Interpret the fixed point number as an unnormalized floating point,
	// send it off to normalization and then call a float2float cast to fix it up

	// The corresponding exponent that correctly places the decimal point is
	// (a_size-1) + a_offset and needs to be biased
	const int exp_unbiased = a_size-1 + a_offset;

//	// Find the maximum absolute exponent, normalization might result in shifting!
//	// MOVED TO FindMaxExponent for compatability with ICC
//	const int exp_unbiased_abs = (exp_unbiased > 0) ? exp_unbiased : -exp_unbiased;
//	const int exp_maxshift = exp_unbiased - (a_size-1);
//	const int exp_maxshift_abs = (exp_maxshift > 0) ? exp_maxshift : -exp_maxshift;
//	const int exp_maxabs = (exp_maxshift_abs > exp_unbiased_abs) ? exp_maxshift_abs : exp_unbiased_abs;
	const int exp_maxabs = _FindMaxExponent<a_size, a_offset>::exp_maxabs;

	// Work out the number of bits we need to represent the exponent
	// (we add 2 to allow the full sign range + 0/NaN special stuff)
	const uint exp_bitsize = MinBitSize<exp_maxabs>::value + 2;

	// Now we need to bias the exponent
	const int exp_bias_val = (1<<(exp_bitsize-1)) - 1;
	const int exp_biased = exp_unbiased + exp_bias_val;
	const varint_u<exp_bitsize> exponent = exp_biased;

	// We need handle cases with 1-bit wide inputs separately as they would correspond
	// to a 0-bit mantissa (with 1 implicit bit) otherwise.
	if (a_size == 1) {
		varint_s<5> mantissa = a.getBitString()[0]
				? ( (a_sign_mode == TWOSCOMPLEMENT) ? varint_s<5>(-1) : varint_s<5>(2) )	// 1.0 (UNS) or -1.0 (TWO's)
				: varint_s<5>(0);															// Zero

		HWFloat<exp_bitsize, 2> normalized = normalize_float<exp_bitsize, 2, false>(false, exponent, mantissa, false, false, false, NULL);
		return cast_float2float<r_esize, r_msize, exp_bitsize, 2, hw_exception>(normalized, exception);
	}
	else {
		// First, expand the mantissa by 3 bits to a signed integer. The result has the form
		// {Sgn}{Int2..1}{Int0,Fractional:-1..-(msize-1)}
		//               { Original fixed point number  }
		varint_s<a_size+3> mantissa = (a_sign_mode == TWOSCOMPLEMENT)
				? varint_s<a_size+3>(varint_s<a_size>(a.getBitString()))
				: varint_s<a_size+3>(varint_u<a_size+3>(a.getBitString()));

		HWFloat<exp_bitsize, a_size> normalized = normalize_float<exp_bitsize, a_size, false>(false, exponent, mantissa, false, false, false, NULL);
		return cast_float2float<r_esize, r_msize, exp_bitsize, a_size, hw_exception>(normalized, exception);
	}
}

CAST_float2fixed(,HWEXCEPT,EXCEPTFLOAT) {
	// Conversion idea: Follow the fixed2fixed casting algorithm, but this time keep in mind that
	// the input offset is variable. We always round TONEARESTEVEN.

	/* Check for special cases ***************************************************************************************/

	if (hw_exception) {
		varint_u<4> except_vector;

		if (a.isInfinity() || a.isNan())
			except_vector[2] = true;

		if (a.isInfinity())
			except_vector[1] = true;

		*exception = ExceptFloat(except_vector);
	}

	if (r_sign_mode == UNSIGNED) {
		if (a.isZero() || a.isNan() || (a.isInfinity() && a.getSign()))
			return HWOffsetFix<r_size, r_offset, r_sign_mode>(varint_u<r_size>(0));

		if (a.isInfinity()) { // && !a.getSign() is implicit
			varint_u<r_size> bits = 0;
			bits = ~bits;
			return HWOffsetFix<r_size, r_offset, r_sign_mode>(bits);
		}
	}
	else {	// r_sign_mode == TWOSCOMPLEMENT
		if (a.isZero())
			return HWOffsetFix<r_size, r_offset, r_sign_mode>(varint_u<r_size>(0));

		if (a.isNan() || (a.isInfinity() && a.getSign())) {
			varint_u<r_size> bits = 0;
			bits[r_size-1] = true;
			return HWOffsetFix<r_size, r_offset, r_sign_mode>(bits);
		}

		if (a.isInfinity()) { // && !a.getSign() is implicit
			varint_u<r_size> bits = 0;
			bits[r_size-1] = true;
			bits--;
			return HWOffsetFix<r_size, r_offset, r_sign_mode>(bits);
		}
	}

	/* Normal Casting ************************************************************************************************/

	// First, let's get the mantissa as the source fixed point, make the "1," explicit
	// and adapt the sign if necessary
	const uint a_size = a_msize+1;
	varint_u<a_size> source = a.getMantissa();
	source[a_msize-1] = true;
	if (a.getSign())
		source = -source;

	varint_u<r_size> result = 0;

	// Do the usual calculations, work out a_offset based on the exponent value
	int a_offset = (int)a.getExponent() - ((1<<(a_esize-1))-1) - (a_msize-1);

	const uint width_low = MIN( MAX(a_offset - r_offset, 0), int(r_size) );
	const uint width_middle = MAX(0,
									MIN(a_offset+int(a_size), r_offset+int(r_size)) -	// bit_left
									MAX(a_offset, r_offset)								// bit_right
								);
	const uint width_high = MIN( MAX(0,
									(r_offset+int(r_size)) -							// out_outset
									(a_offset+int(a_size))								// in_outset
								), int(r_size) );

	// Always work out the hardware exceptions, we need to return special values if we overflow!
	varint_u<4> except_vector;

	// The input's and output's MSBs
	const int in_msb = a_offset + a_size - 1;
	const int out_msb = r_offset + r_size - 1;

	if (out_msb > in_msb)
		; // Cannot overflow
	else if (out_msb == in_msb) {
		if (r_sign_mode == UNSIGNED)
			except_vector[1] = a.getSign();
	}
	else {	// out_msb < in_msb ~> Chopped off bits
		/*
		 * (ii)  TWOSCOMPLEMENT->UNSIGNED: cannot chop more than 1 bit, as MSB-1 is guaranteed '1'
		 * (iv)  TWOSCOMPLEMENT->TWOSCOMPLEMENT:
		 *       (a) In and out overlap:   add extra chopped bit, either all chopped bits 0 or all chopped bits 1
		 *       (b) No Overlap:           If the number was zero, we would have returned above ~> guarenteed overflow
		 */
		if (r_sign_mode == UNSIGNED) { // (ii)
			except_vector[1] = ((in_msb-out_msb) > 1);
		}
		else if (width_middle > 0) { // && in=out=Two'sComp ~> (iv/a)
			const int bits_chopped = MIN(in_msb-out_msb+1, int(a_size));
			varint_s<a_size> lost_bits = source;	// Convert to signed for easy all-ones check
			lost_bits >>= (a_size - bits_chopped);
			bool all_zeros = ! ((bool)lost_bits);
			bool all_ones = ! ((bool)~lost_bits);
			if (! (all_zeros || all_ones))
				except_vector[1] = true;
		}
		else {	// (iv/b)
			except_vector[1] = true;
		}
	}

	if (hw_exception) {
		*exception = ExceptFloat(except_vector);
	}

	// Check for overflow
	if (except_vector[1]) {
		if (r_sign_mode == UNSIGNED) {
			if (a.getSign())
				return HWOffsetFix<r_size, r_offset, r_sign_mode>(varint_u<r_size>(0));
			else {
				varint_u<r_size> bits = 0;
				bits = ~bits;
				return HWOffsetFix<r_size, r_offset, r_sign_mode>(bits);
			}
		}
		else {
			if (a.getSign()) {
				varint_u<r_size> bits = 0;
				bits[r_size-1] = true;
				return HWOffsetFix<r_size, r_offset, r_sign_mode>(bits);
			}
			else {
				varint_u<r_size> bits = 0;
				bits[r_size-1] = true;
				bits--;
				return HWOffsetFix<r_size, r_offset, r_sign_mode>(bits);
			}
		}
	}


	// Assign the low bits to constant zero
	// NOTHING																				// hehe, this comes for free!

	// Assign the middle bits from the matching input slice
	if (width_middle != 0) {
		uint right_bit_trg = width_low;
		uint right_bit_src = MAX(r_offset-a_offset, 0);
		uint length = width_middle;

		while (length > 64) {
			result.set_bits(right_bit_trg, 64, source.get_bits(right_bit_src, 64));
			length -= 64;
			right_bit_src += 64;
			right_bit_trg += 64;
		}
		result.set_bits(right_bit_trg, length, source.get_bits(right_bit_src, 64));
	}

	// Assign the high bits: We always sign extend if the numbers overlap
	if (width_high != 0 && width_middle > 0)
			// 0s are default, we need to set 1s explicitly if input is negative
			if (a.getSign()) {
				const uint64_t ones = -1;
				uint right_trg   = width_low+width_middle;
				uint length_left = r_size - right_trg;

				while (length_left > 64) {
					result.set_bits(right_trg, 64, ones);
					length_left -= 64;
					right_trg += 64;
				}
				result.set_bits(right_trg, length_left, ones);
			}

	// Special case: only occurs if the result is UNSIGNED, but this conversion always
	// assumes a TWOSCOMPLEMENT result! -> Nothing to do.
	// ...

	// General rounding pre-condition: bits are cut off and the numbers overlap
	if (! (r_offset > a_offset && width_middle > 0))
		return HWOffsetFix<r_size, r_offset, r_sign_mode>(result);

	// If we've made it to here, we need to round somehow...
	bool round_up = false;

	int cut_bits = r_offset - a_offset;
	if (cut_bits == 1)
		round_up = source[1] && source[0];
	else if (cut_bits == 2)
		round_up = source[1] && (source[0] || source[2]);
	else {
		bool drop_lower = source << ((a_size - cut_bits) + 1);
		bool drop_msb = source[cut_bits-1];
		bool keep_lsb = source[cut_bits];
		round_up = drop_msb && (keep_lsb || drop_lower);
	}

	if (round_up)
		result++;

	return HWOffsetFix<r_size, r_offset, r_sign_mode>(result);
}


// Arithmetic ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

NEG_float(inline) {
	return HWFloat<r_esize, r_msize>(! a.getSign(), a.getExponent(), a.getMantissa());
}

ADD_float(,HWEXCEPT,EXCEPTFLOAT) {
	if (hw_exception) {
		varint_u<4> except_vector;

		// Check for invalid op. If found, we'll return from this method
		// in one of the special cases
		if ( a.isInfinity() && b.isInfinity() && a.getSign() != b.getSign() )
			except_vector[2] = true;

		*exception = ExceptFloat(except_vector);
	}

	// Check for zero (denormalised numbers are implicitly converted to zero)
	const bool isZeroDenormA = a.isZeroOrDenormal();
	const bool isZeroDenormB = b.isZeroOrDenormal();

	if (isZeroDenormA & isZeroDenormB)
		return HWFloat<r_esize, r_msize>(a.getSign() && b.getSign(), varint_u<r_esize>(0), varint_u<r_msize-1>(0));
	if (isZeroDenormA)
		return b;
	if (isZeroDenormB)
		return a;

	// Check for NaN and Infinity
	if (a.isNan() || b.isNan())
		return HWFloat<r_esize, r_msize>::NaN();
	else if (a.isInfinity() && b.isInfinity())
		return (a.getSign() == b.getSign()) ? a : HWFloat<r_esize, r_msize>::NaN();
	else if (a.isInfinity())
		return a;
	else if (b.isInfinity())
		return b;

	// If we've made it to here, it's a normal addition. Find the number with the larger exponent
	bool isExpALargerB = a.getExponent() >= b.getExponent();
	const HWFloat<r_esize, r_msize> &lrg = (isExpALargerB) ? a : b;
	const HWFloat<r_esize, r_msize> &sml = (isExpALargerB) ? b : a;

	// Work out the larger-exponent number's mantissa in computable form, easy!
	varint_s<r_msize+3> lrg_mantissa = lrg.getMantissa();
	lrg_mantissa[r_msize-1] = true;									// Make the '1,' explicit
	if (lrg.getSign())
		lrg_mantissa = -lrg_mantissa;

	// Work out the smaller-exponent number's mantissa in computable form, need to shift!
	varint_s<r_msize+3> sml_mantissa = sml.getMantissa();
	sml_mantissa[r_msize-1] = true;									// Make the '1,' explicit
	if (sml.getSign())
		sml_mantissa = -sml_mantissa;

	int shift_by = lrg.getExponent()-sml.getExponent();

	bool guard_bit = false;
	if (shift_by >= 1)	// The rightmost bit to get lost
		guard_bit = sml_mantissa[shift_by-1];

	bool round_bit = false;
	if (shift_by >= 2)	// The two rightmost bits to get lost
		round_bit = sml_mantissa[shift_by-2];

	bool sticky_bit = false;
	if (shift_by >= 3)	// The other bits to get lost
		sticky_bit = sml_mantissa << uint( (r_msize+3) - (shift_by-2) );

	sml_mantissa >>= shift_by;

	// Add the mantissas
	varint_s<r_msize+3> mantissa = lrg_mantissa + sml_mantissa;

	return normalize_float<r_esize, r_msize, hw_exception>(false, lrg.getExponent(), mantissa, guard_bit, round_bit, sticky_bit, exception);
}

SUB_float(inline,HWEXCEPT,EXCEPTFLOAT) {
	return add_float<r_esize,r_msize,hw_exception>(a, b.isNan() ? b : neg_float(b), exception);
}

MUL_float(,HWEXCEPT,EXCEPTFLOAT) {
	const bool isZeroDenormA = a.isZeroOrDenormal();
	const bool isZeroDenormB = b.isZeroOrDenormal();

	if (hw_exception) {
		varint_u<4> except_vector;

		// Check for invalid op. If found, we'll return from this method
		// in one of the special cases
		if ( (isZeroDenormA && b.isInfinity()) ||
				(isZeroDenormB && a.isInfinity()) )
			except_vector[2] = true;

		*exception = ExceptFloat(except_vector);
	}

	// Check for NaN
	if (a.isNan() || b.isNan())
		return HWFloat<r_esize, r_msize>::NaN();

	// Check for Infinity
	if (a.isInfinity() && isZeroDenormB)
		return HWFloat<r_esize, r_msize>::NaN();
	if (b.isInfinity() && isZeroDenormA)
		return HWFloat<r_esize, r_msize>::NaN();
	if (a.isInfinity())
		return HWFloat<r_esize, r_msize>(a.getSign() ^ b.getSign(), a.getExponent(), a.getMantissa());
	if (b.isInfinity())
		return HWFloat<r_esize, r_msize>(a.getSign() ^ b.getSign(), b.getExponent(), b.getMantissa());

	// Check for Zero (denormalised numbers are implicitly converted to zero)
	if (isZeroDenormA | isZeroDenormB)
		return HWFloat<r_esize, r_msize>(a.getSign() ^ b.getSign(), varint_u<r_esize>(0), varint_u<r_msize-1>(0));

	// If we've made it to here, it's a normal multiplication, first add the exponents
	varint_u<r_esize+1> exponent_a = a.getExponent();

	// Un-bias the second exponent
	varint_u<r_esize> exp_bias;
	exp_bias[r_esize-1] = true;
	--exp_bias;

	varint_u<r_esize + 1> exp_delta = varint_u<r_esize+1>(b.getExponent()) - varint_u<r_esize+1>(exp_bias);
	varint_u<r_esize + 1> exponent = exponent_a + exp_delta;

	// Now multiply the mantissas, first make the "1," explicit
	varint_u<2*r_msize> mantissa_a = a.getMantissa();
	mantissa_a[r_msize-1] = true;

	varint_u<2*r_msize> mantissa_b = b.getMantissa();
	mantissa_b[r_msize-1] = true;

	varint_u<2*r_msize> mantissa_mult = mantissa_a * mantissa_b;

	// The highest 2 Bit are Integer, bits msize-4..0 give the sticky bit and
	// bit msize-3 is the round bit and msize-2 the guard bit
	varint_s<r_msize+3> mantissa = mantissa_mult >> (r_msize-1);
	bool guard_bit = mantissa_mult[r_msize-2];
	bool round_bit = mantissa_mult[r_msize-3];
	bool sticky_bit = mantissa_mult << ( (2*r_msize) - (r_msize-3) );

	HWFloat<r_esize+1, r_msize> normalized = normalize_float<r_esize+1, r_msize, hw_exception>(a.getSign() ^ b.getSign(), exponent, mantissa, guard_bit, round_bit, sticky_bit, exception);

	exponent = normalized.getExponent();

	if(exponent[r_esize]) {
		// Underflow
		if(exp_delta[r_esize]) {
			if (hw_exception)
				*exception = ExceptFloat(1);
					return HWFloat<r_esize, r_msize>(a.getSign() ^ b.getSign(), varint_u<r_esize>(0), varint_u<r_msize-1>(0));
		}
		// Overflow
		else {
			if (hw_exception)
				*exception = ExceptFloat(2);
			return HWFloat<r_esize, r_msize>::Infty(a.getSign() ^ b.getSign());
		}
	}

	if(exponent == varint_u<r_esize+1>((1 << r_esize)-1)) {
		if (hw_exception)
			*exception = ExceptFloat(2);
		return HWFloat<r_esize, r_msize>::Infty(a.getSign() ^ b.getSign());
	}

	return HWFloat<r_esize, r_msize>(varint_u<1>(normalized.getSign()), varint_u<r_esize>(normalized.getExponent().get_bits(0, r_esize)), varint_u<r_msize>(normalized.getMantissa()));
}

DIV_float(inline,HWEXCEPT,EXCEPTFLOAT) {
	double a_dbl = a.getValueAsDouble();
	double b_dbl = b.getValueAsDouble();

	// Save previous env, clear exceptions, set rounding mode to zero.
	fenv_t backup_env;
	fegetenv(&backup_env);
	feclearexcept(FE_ALL_EXCEPT);
	if(r_esize == 11 && r_msize == 53)
		fesetround(FE_TONEAREST);
	else
		fesetround(FE_TOWARDZERO);
	volatile double r_dbl = a_dbl / b_dbl;
	int inexact = fetestexcept(FE_INEXACT);
	fesetenv(&backup_env);

	// Check for NaN
	if (a.isNan() || b.isNan()) {
		if (hw_exception)
			*exception = ExceptFloat(0);
		return HWFloat<r_esize, r_msize>::NaN();
	}

	HWFloat<11,53> r_hw_dbl = HWFloat<11,53>(r_dbl);
	HWFloat<r_esize,r_msize> r_real = cast_float2float<r_esize,r_msize,11,53,hw_exception>(r_hw_dbl, exception);
	varint_s<r_msize+3> mantissa(r_hw_dbl.getMantissa().get_bits(53-r_msize, r_msize));
	mantissa[r_msize-1] = true;


	if (hw_exception) {
		varint_u<4> except_vector = exception->getBitString();

		// Check for INValid op
		if ( (a.isZero() && b.isZero()) ||
				(a.isInfinity() && b.isInfinity()) )
			except_vector[2] = true;

		// Check for DIV-0
		if ( b.isZero() && !(a.isZero() || a.isInfinity()) )
			except_vector[3] = true;

		*exception = ExceptFloat(except_vector);
	}

	if(r_real.isNanInfinity()) {
		return r_real;
	}
	else {
		bool guard_bit = (r_msize<53) ? (bool)r_hw_dbl.getMantissa()[53-r_msize-1] : false;
		bool round_bit = (r_msize<52) ? (bool)r_hw_dbl.getMantissa()[53-r_msize-2] : false;
		bool sticky_bit = inexact || r_hw_dbl.getMantissa().get_bits(0, 53-r_msize);
		return normalize_float<r_esize, r_msize, hw_exception>(r_real.getSign(), r_real.getExponent(), mantissa, guard_bit, round_bit, sticky_bit, exception);
	}
}

SQRT_float(inline,HWEXCEPT,EXCEPTFLOAT) {
	double a_dbl = a.getValueAsDouble();

	// Save previous env, clear exceptions, set rounding mode to zero.
	fenv_t backup_env;
	fegetenv(&backup_env);
	feclearexcept(FE_ALL_EXCEPT);
	if(r_esize == 11 && r_msize == 53)
		fesetround(FE_TONEAREST);
	else
		fesetround(FE_TOWARDZERO);
	volatile double r_dbl = sqrt(a_dbl);
	int inexact = fetestexcept(FE_INEXACT);
	fesetenv(&backup_env);

	// Check for NaN
	if (a.isNan()) {
		if (hw_exception)
			*exception = ExceptFloat(0);
		return HWFloat<r_esize, r_msize>::NaN();
	}

	HWFloat<11,53> r_hw_dbl = HWFloat<11,53>(r_dbl);
	HWFloat<r_esize,r_msize> r_real = cast_float2float<r_esize,r_msize,11,53,hw_exception>(r_hw_dbl, exception);
	varint_s<r_msize+3> mantissa(r_hw_dbl.getMantissa().get_bits(53-r_msize, r_msize));
	mantissa[r_msize-1] = true;


	if (hw_exception) {
		varint_u<4> except_vector = exception->getBitString();

		// Check for INValid op
		if ( a.getSign() )
			except_vector[2] = true;

		*exception = ExceptFloat(except_vector);
	}

	if(r_real.isNanInfinity()) {
		return r_real;
	}
	else {
		bool guard_bit = (r_msize<53) ? (bool)r_hw_dbl.getMantissa()[53-r_msize-1] : false;
		bool round_bit = (r_msize<52) ? (bool)r_hw_dbl.getMantissa()[53-r_msize-2] : false;
		bool sticky_bit = inexact || r_hw_dbl.getMantissa().get_bits(0, 53-r_msize);
		return normalize_float<r_esize, r_msize, hw_exception>(r_real.getSign(), r_real.getExponent(), mantissa, guard_bit, round_bit, sticky_bit, exception);
	}
}

// Comparison ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

EQ_float(inline) {
	// Check for NaNs
	if (a.isNan() || b.isNan())
		return const_false();

	// Check for Zeros
	if (a.isZero())
		return const_bool(b.isZero());

	// Otherwise check for bitwise equality
	return const_bool(
			a.getSign() == b.getSign() &&
			a.getExponent() == b.getExponent() &&
			a.getMantissa() == b.getMantissa()
		);
}

NEQ_float(inline) {
	// Check for NaNs
	if (a.isNan() || b.isNan())
		return const_true();

	// Check for Zeros
	if (a.isZero())
		return const_bool(! b.isZero());

	// Otherwise check for bitwise inequality
	return const_bool(
			a.getSign() != b.getSign() ||
			a.getExponent() != b.getExponent() ||
			a.getMantissa() != b.getMantissa()
		);
}

GT_float() {
	// Check for NaNs
	if (a.isNan() || b.isNan())
		return const_false();

	// Check for Zeros
	if (a.isZero())
		return const_bool(b.getSign() && !b.isZero());
	if (b.isZero())
		return const_bool(!a.getSign() && !a.isZero());

	// A non-zero positive
	if (!a.getSign()) {
		// B non-zero negative?
		if (b.getSign())
			return const_true();

		// A's exponent > B's exponent?
		if (a.getExponent() > b.getExponent())
			return const_true();
		// A's exponent < B's exponent?
		if (a.getExponent() < b.getExponent())
			return const_false();

		// Sign+Exp is equal, A's mantissa > B's mantissa?
		if (a.getMantissa() > b.getMantissa())
			return const_true();

		// Sign+Exp is equal, A's mantissa <= B's mantissa ~~> return false
		return const_false();
	}

	// A non-zero negative! Is B positive?
	if (!b.getSign())
		return const_false();

	// A's exponent < B's exponent?
	if (a.getExponent() < b.getExponent())
		return const_true();
	// A's exponent > B's exponent?
	if (a.getExponent() > b.getExponent())
		return const_false();

	// Sign+Exp equal, A's mantissa < B's mantissa?
	if (a.getMantissa() < b.getMantissa())
		return const_true();

	// Sign+Exp equal, A's mantissa >= B's mantissa ~~> return false
	return const_false();
}

GTE_float() {
	// Check for NaNs
	if (a.isNan() || b.isNan())
		return const_false();

	// Check for Zeros
	if (a.isZero())
		return const_bool(b.getSign() || b.isZero());
	if (b.isZero())
		return const_bool(!a.getSign() || a.isZero());

	// A non-zero positive
	if (!a.getSign()) {
		// B non-zero negative?
		if (b.getSign())
			return const_true();

		// A's exponent > B's exponent?
		if (a.getExponent() > b.getExponent())
			return const_true();
		// A's exponent < B's exponent?
		if (a.getExponent() < b.getExponent())
			return const_false();

		// Sign+Exp is equal, A's mantissa >= B's mantissa?
		if (a.getMantissa() >= b.getMantissa())
			return const_true();

		// Sign+Exp is equal, A's mantissa < B's mantissa ~~> return false
		return const_false();
	}

	// A non-zero negative! Is B positive?
	if (!b.getSign())
		return const_false();

	// A's exponent < B's exponent?
	if (a.getExponent() < b.getExponent())
		return const_true();
	// A's exponent > B's exponent?
	if (a.getExponent() > b.getExponent())
		return const_false();

	// Sign+Exp equal, A's mantissa <= B's mantissa?
	if (a.getMantissa() <= b.getMantissa())
		return const_true();

	// Sign+Exp equal, A's mantissa > B's mantissa ~~> return false
	return const_false();
}

LT_float() {
	// Check for NaNs
	if (a.isNan() || b.isNan())
		return const_false();

	// Check for Zeros
	if (a.isZero())
		return const_bool(!b.getSign() && !b.isZero());
	if (b.isZero())
		return const_bool(a.getSign() && !a.isZero());

	// A non-zero positive
	if (!a.getSign()) {
		// B non-zero negative?
		if (b.getSign())
			return const_false();

		// A's exponent < B's exponent?
		if (a.getExponent() < b.getExponent())
			return const_true();
		// A's exponent > B's exponent?
		if (a.getExponent() > b.getExponent())
			return const_false();

		// Sign+Exp is equal, A's mantissa < B's mantissa?
		if (a.getMantissa() < b.getMantissa())
			return const_true();

		// Sign+Exp is equal, A's mantissa >= B's mantissa ~~> return false
		return const_false();
	}

	// A non-zero negative! Is B positive?
	if (!b.getSign())
		return const_true();

	// A's exponent > B's exponent?
	if (a.getExponent() > b.getExponent())
		return const_true();
	// A's exponent < B's exponent?
	if (a.getExponent() < b.getExponent())
		return const_false();

	// Sign+Exp equal, A's mantissa > B's mantissa?
	if (a.getMantissa() > b.getMantissa())
		return const_true();

	// Sign+Exp equal, A's mantissa <= B's mantissa ~~> return false
	return const_false();
}

LTE_float() {
	// Check for NaNs
	if (a.isNan() || b.isNan())
		return const_false();

	// Check for Zeros
	if (a.isZero())
		return const_bool(!b.getSign() || b.isZero());
	if (b.isZero())
		return const_bool(a.getSign() || a.isZero());

	// A non-zero positive
	if (!a.getSign()) {
		// B non-zero negative?
		if (b.getSign())
			return const_false();

		// A's exponent < B's exponent?
		if (a.getExponent() < b.getExponent())
			return const_true();
		// A's exponent > B's exponent?
		if (a.getExponent() > b.getExponent())
			return const_false();

		// Sign+Exp is equal, A's mantissa <= B's mantissa?
		if (a.getMantissa() <= b.getMantissa())
			return const_true();

		// Sign+Exp is equal, A's mantissa > B's mantissa ~~> return false
		return const_false();
	}

	// A non-zero negative! Is B positive?
	if (!b.getSign())
		return const_true();

	// A's exponent > B's exponent?
	if (a.getExponent() > b.getExponent())
		return const_true();
	// A's exponent < B's exponent?
	if (a.getExponent() < b.getExponent())
		return const_false();

	// Sign+Exp equal, A's mantissa >= B's mantissa?
	if (a.getMantissa() >= b.getMantissa())
		return const_true();

	// Sign+Exp equal, A's mantissa < B's mantissa ~~> return false
	return const_false();
}

} /* end namespace maxcompilersim */

#endif /* HWFLOAT_IMPL_H_ */
