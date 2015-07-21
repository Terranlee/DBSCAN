#ifndef HWOFFSETFIX_IMPL_H_
#define HWOFFSETFIX_IMPL_H_

#include "HWOffsetFix_decl.h"

// Helper Macros: these create compiler recognized constant results for
// constant inputs, unlike std::max and std::min
#define MAX(a, b) ( ((a)>(b)) ? (a) : (b) )
#define MIN(a, b) ( ((a)>(b)) ? (b) : (a) )

// See src/com/maxeler/photon/hw/EntityStructuralFixPointUtils.java - encompassingType()
#define ENCOMP(a_size, a_offset, a_sign_mode, b_size, b_offset, b_sign_mode, force_signed)										\
	const SignMode encomp_sign_mode = (a_sign_mode == b_sign_mode && !force_signed) ? a_sign_mode : TWOSCOMPLEMENT;				\
	const int encomp_offset = MIN(a_offset, b_offset);																			\
	const int encomp_tmp_max_msb = MAX(a_offset+int(a_size), b_offset+int(b_size));												\
	const int encomp_tmp_size = encomp_tmp_max_msb - encomp_offset;																\
	const int encomp_size =																										\
			((a_sign_mode == UNSIGNED && encomp_sign_mode == TWOSCOMPLEMENT && a_offset+int(a_size) >= b_offset+int(b_size)) ||	\
			 (b_sign_mode == UNSIGNED && encomp_sign_mode == TWOSCOMPLEMENT && b_offset+int(b_size) >= a_offset+int(a_size)) )	\
			 ? encomp_tmp_size + 1 : encomp_tmp_size


namespace maxcompilersim {

// Constructors //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <uint size, int offset, SignMode sign_mode> inline
HWOffsetFix<size,offset,sign_mode>::HWOffsetFix(varint_u<size> raw_bits)
	: m_bits(raw_bits)
	{ }

template <uint size, int offset, SignMode sign_mode> inline
HWOffsetFix<size,offset,sign_mode>::HWOffsetFix(double v) {
	HWFloat<11,53> float_dbl(v);
	HWOffsetFix<size,offset,sign_mode> trueFix = cast_float2fixed<size,offset,sign_mode>(float_dbl);
	*this = trueFix;
}

template <uint size, int offset, SignMode sign_mode> inline
HWOffsetFix<size,offset,sign_mode>::HWOffsetFix(long v) {
	HWOffsetFix<64, 0, TWOSCOMPLEMENT> longFix( (varint_u<64>)v );
	HWOffsetFix<size, offset, sign_mode> trueFix =
			cast_fixed2fixed<size, offset, sign_mode, TRUNCATE, 64, 0, TWOSCOMPLEMENT>(longFix);

	*this = trueFix;
}

template <uint size, int offset, SignMode sign_mode> inline
HWOffsetFix<size,offset,sign_mode>::HWOffsetFix()
	// TODO Uninitialized value
	{ }

// Getters ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <uint size, int offset, SignMode sign_mode> inline
varint_u<size> HWOffsetFix<size,offset,sign_mode>::getBitString() const {
	return m_bits;
}

template <uint size, int offset, SignMode sign_mode> inline
double HWOffsetFix<size,offset,sign_mode>::getValueAsDouble() const {
	return (cast_fixed2float<11,53>(*this)).getValueAsDouble();
}

template <uint size, int offset, SignMode sign_mode> inline
long HWOffsetFix<size,offset,sign_mode>::getValueAsLong() const {
	HWOffsetFix<64, 0, TWOSCOMPLEMENT> longFix =
			cast_fixed2fixed<64, 0, TWOSCOMPLEMENT, TONEAREVEN>(*this);
	return (long)longFix.getBitString();
}

template <uint size, int offset, SignMode sign_mode> inline
bool HWOffsetFix<size,offset,sign_mode>::getValueAsBool() const {
	return (bool)m_bits;
}

// Constants /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CONST_true(inline)	{ return HWBool( varint_u<1>((uint64_t)1) );			}
CONST_false(inline)	{ return HWBool( varint_u<1>((uint64_t)0) );			}
CONST_bool(inline)	{ return v ? const_true() : const_false();	}

// Helpers ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


template<
	uint right_bit_trg, uint right_bit_src, uint length,
	uint size_trg, uint size_src
> inline
void copy_bits(varint_u<size_trg>& target, const varint_u<size_src>& source) {
	if (length <= 64)
		target.set_bits(right_bit_trg, length, source.get_bits(right_bit_src, length));
	else {
		uint right_src   = right_bit_src;
		uint right_trg   = right_bit_trg;
		uint length_left = length;

		while (length_left > 64) {
			target.set_bits(right_trg, 64, source.get_bits(right_src, 64));
			length_left -= 64;
			right_src += 64;
			right_trg += 64;
		}
		target.set_bits(right_trg, length_left, source.get_bits(right_src, 64));
	}
}

template <
	uint right_bit_trg, uint size_trg
> inline
void one_extend(varint_u<size_trg>& target) {
	const uint length = size_trg - right_bit_trg;
	const uint64_t ones = -1;

	if (length <= 64)
		target.set_bits(right_bit_trg, length, ones);
	else {
		uint right_trg   = right_bit_trg;
		uint length_left = length;

		while (length_left > 64) {
			target.set_bits(right_trg, 64, ones);
			length_left -= 64;
			right_trg += 64;
		}
		target.set_bits(right_trg, length_left, ones);
	}
}

// See src/com/maxeler/photon/hw/EntityStructuralFixPointUtils.java - extendInput()
template <
	uint big_size, int big_offset, SignMode big_sign_mode,
	uint cur_size, int cur_offset, SignMode cur_sign_mode
> inline
varint_u<big_size> extend_input(const varint_u<cur_size>& source) {
	const int pad_right = cur_offset - big_offset;
	const int pad_left = (big_offset + (int)big_size) - (cur_offset + (int)cur_size);

	varint_u<big_size> result(source);

	if (pad_right > 0)
		result <<= pad_right;

	// Sign Extend if required
	if (pad_left > 0 && !(cur_sign_mode == UNSIGNED)) {
		if (source[cur_size-1] == true)
			one_extend<cur_size+pad_right>(result);
	}

	return result;
}

// Sign extend
// See src/com/maxeler/photon/hw/MaxDCFixedCast.java
template <
	uint right_bit_trg, SignMode sign_mode, SignMode result_mode, uint additional_width, uint size_trg
> inline
void pad_msb(varint_u<size_trg>& target) {
	uint64_t ext;

	if ( (int) right_bit_trg <= 0 || (int) additional_width == 0) {
		return;
	}
	else {
		if (sign_mode == TWOSCOMPLEMENT && result_mode != UNSIGNED && target[right_bit_trg - 1])
			ext = -1; // all ones
		else
			ext = 0;  // all zeros
	}

	if (additional_width <= 64)
		target.set_bits(right_bit_trg, additional_width, ext);
	else {
		uint right_trg   = right_bit_trg;
		uint length_left = additional_width;

		while (length_left > 64) {
			target.set_bits(right_trg, 64, ext);
			length_left -= 64;
			right_trg += 64;
		}
		target.set_bits(right_trg, length_left, ext);
	}
}



// Casting ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
 * We define two internal casting methods _cast_fixed2fixed. One really casts, the other method
 * is intended for identical input and output types and merely passes through its input.
 *
 * Since C++ doesn't allow partial template specialization on methods, we have to rely on templetized
 * method overloading to select between the two instantiations. This is accomplished by an additional
 * input argument real_cast which is either of type Int2Type<false> for pass-through or
 * Int2Type<true> for real casting.
 *
 * The method for public use CAST_fixed2fixed works out the casting type during compile time based
 * on its template arguments and then selects the appropriate internal implementation.
 * 
 * Both methods (functions) are defined inline as we hope that this will allow proper
 * compilation with the -fno-implicit-instantation flag.
 * Theory goes like this: If gcc encounters the cast_fixed2fixed function in a file that is being
 * compiled with the no-implicit-instantiation flag, it will - depending on the optimization level 
 * and the phase of the moon - instantiate the cast_fixed2fixed template (because it is inlined) but
 * not the _cast_fixed2fixed(..., true) template if it was declared non-inline. Hence the call to
 * _cast_fixed2fixed will remain unresolved in that object file.
 * During explicit template instantiation of cast_fixed2fixed, however, gcc might - again depending
 * on the optimization level and the phase of the moon - optimize away the function call to
 * _cast_fixed2fixed(..., true). (?!)  This then results in an object file without an explicit 
 * _cast_fixed2fixed symbol and, consequently, in a linker error, as _cast_fixed2fixed is undefined.
 * Marking _cast_fixed2fixed(..., true) inline tells gcc to emit code for the function when it inlined
 * cast_fixed2fixed in the first place.
 * Well, at least that's the theory. Does this make any sense at all?
 */

template <
	uint r_size, int r_offset, SignMode r_sign_mode,
	RoundMode round_mode,
	uint a_size, int a_offset, SignMode a_sign_mode
	HWEXCEPT
> inline
HWOffsetFix<r_size, r_offset, r_sign_mode>
_cast_fixed2fixed(const HWOffsetFix<a_size, a_offset, a_sign_mode> &a, EXCEPTOVERFLOW, Int2Type<false> real_cast) {
	if (hw_exception)
		*exception = ExceptOverflow(0);

	return a;
}

template <
	uint r_size, int r_offset, SignMode r_sign_mode,
	RoundMode round_mode,
	uint a_size, int a_offset, SignMode a_sign_mode
	HWEXCEPT
> inline
HWOffsetFix<r_size, r_offset, r_sign_mode>
_cast_fixed2fixed(const HWOffsetFix<a_size, a_offset, a_sign_mode> &a, EXCEPTOVERFLOW, Int2Type<true>) {
	// See src/com/maxeler/photon/hw/MaxDCFixedCast.java for details on casting

	const int in_lsb = a_offset;
	const int out_lsb = r_offset;
	const int in_msb =  a_offset + (int) a_size - 1;
	const int out_msb = r_offset + (int) r_size - 1;

	const int in_msb_adjusted =
			(in_lsb < out_lsb) ?
					((out_lsb <= in_msb + 1) ? (in_msb + 1) /* Case (c1) */ : (out_lsb) /* Case (c2) */) :
					(in_msb) /* All other cases */;

	// Size for the intermediate result
	const uint max_size = MAX(in_msb_adjusted, out_msb) -
			              MIN(in_lsb, out_lsb) + 1;

	varint_u<a_size> in = a.getBitString();
	// Intermediate result
	varint_u<max_size> out;
	varint_u<r_size> real_out;

	// Handle LSBs and rounding

	// Case (a) LSBs are the same
	//  i:  ====|
	//  o:  ====|
	if (in_lsb == out_lsb) {

		// Nothing to do
		out = in;

		if (a_sign_mode == TWOSCOMPLEMENT)
			pad_msb<a_size, a_sign_mode, r_sign_mode, max_size - a_size>(out);
	}
	// Case (b) Output LSB further right
	//  i:  ===|
	//  o:  ======|
	else if (in_lsb > out_lsb){

		// Pad LSBs with zero
		const int additional_width = MAX(in_lsb - out_lsb, 0);

		copy_bits<additional_width, 0, a_size>(out, in);

		if (a_sign_mode == TWOSCOMPLEMENT)
			pad_msb<a_size + additional_width, a_sign_mode, r_sign_mode, max_size - (a_size + additional_width)>(out);
	}
	// Case (c) Output LSB further left
	else {

		// Case (c1) Output LSB right of input MSB
		//  i:    |======|
		//  o:  =====|
		//
		//  i:     |====|
		//  o:  ===|
		if (out_lsb <= in_msb + 1) {

			// Cut off LSBs if necessary
			const int cutoff_offset = MAX(out_lsb - in_lsb, 1);
			const int cutoff_width = MAX((int) a_size - cutoff_offset, 0);

			if (cutoff_width > 0)
				out = in.slice<cutoff_offset, cutoff_width>();

			// in_msb_adjusted = in_msb + 1 (set above)

			if (a_sign_mode == TWOSCOMPLEMENT) {

				if (cutoff_width == 0) {

					// Attach a guard MSB, then sign extend if we didn't cut off LSBs (i.e no overlap)
					out[0] = (bool) in[a_size-1];
					pad_msb<1, a_sign_mode, r_sign_mode, max_size - 1>(out);
				}
				else {

					// Sign extend
					pad_msb<cutoff_width, a_sign_mode, r_sign_mode, max_size - cutoff_width>(out);
				}
			}

			bool round_up;

			// Decide rounding
			switch (round_mode) {
				case TRUNCATE:
					round_up = false;
					break;
				case TONEAR:
					round_up = in[cutoff_offset - 1];
					break;
				case TONEAREVEN:
		            if (cutoff_offset == 1)
		            	round_up = in[1] && in[0];
		            else if (cutoff_offset == 2)
		            	round_up = in[1] && (in[0] || in[2]);
		            else {
		            	bool drop_lower = in.slice<0, cutoff_offset - 1>();
		            	bool drop_msb = in[cutoff_offset - 1];
		            	bool keep_lsb = out[0];
		            	round_up = drop_msb && (keep_lsb || drop_lower);
		            }
		            break;
			}

			// Round
			if (round_up)
				out++;
		}
		// Case (c2) Output LSB left of input MSB
		//  i:        |======|
		//  o:  ===|
		else {

			// Set output LSB
			// in_msb_adjusted = out_lsb (set above)

			if (a_sign_mode == UNSIGNED) {

				// out = 0 -> Nothing to do
			}
			else {
				if (round_mode == TRUNCATE) {

					// Use input MSB
					real_out[0] = (bool) in[a_size - 1];
					if (a_sign_mode == TWOSCOMPLEMENT)
						pad_msb<1, a_sign_mode, r_sign_mode, max_size - 1>(real_out);
				}
				else {

					// out = 0 -> Nothing to do
				}
			}

			if (hw_exception)
				*exception = ExceptOverflow(varint_u<1>(0l));

			return HWOffsetFix<r_size, r_offset, r_sign_mode>(real_out);
		}
	}

	// Handle MSBs and overflows
	varint_u<1> overflow;

	// Case (a) MSBs are the same
	//  i:  |====
	//  o:  |====
	if (in_msb_adjusted == out_msb) {

		// Case (a1) Sign modes are the same
		if (a_sign_mode == r_sign_mode) {

			// Nothing to do
		}
		// Case (a2) Sign mode changes
		else {

			// Overflow if MSB is not 0
			if (hw_exception)
				overflow[0] = (bool) out[out_msb];
		}

		real_out = out;
	}
	// Case (b) Output MSB further right
	//  i:  |=======
	//  o:     |====
	else if (in_msb_adjusted > out_msb){

		// We're going to cut off MSBs from the input
		const int cutoff_index = MAX(r_size, 1);
		const int cutoff_width = MAX(max_size - cutoff_index, 0);
		//const int cutoff_width = MAX(in_msb_adjusted - out_msb, 0);

		// Cases (b1) UNSIGNED -> UNSIGNED and (b3) TWOSCOMPLEMENT -> UNSIGNED
		if ((a_sign_mode == UNSIGNED && r_sign_mode == UNSIGNED) ||
			 (a_sign_mode == TWOSCOMPLEMENT && r_sign_mode == UNSIGNED))
		{
			// Overflow if cutting off anything != 0
			if (hw_exception) {
				varint_u<cutoff_width> cutoff = out.slice<cutoff_index, cutoff_width>();

				overflow[0] = (bool) cutoff;
			}
		}
		// Case (b2) UNSIGNED -> TWOSCOMPLEMENT
		else if (a_sign_mode == UNSIGNED && r_sign_mode == TWOSCOMPLEMENT) {

			// Overflow as (b1) but include output MSB in overflow check
			if (hw_exception) {
				varint_u<cutoff_width + 1> cutoff = out.slice<cutoff_index - 1, cutoff_width + 1>();

				overflow[0] = (bool) cutoff;
			}
		}
		// Case (b4) TWOSCOMPLEMENT -> TWOSCOMPLEMENT
		else {

			// Overflow as (b2) but bits can be either all 1 or all 0
			if (hw_exception) {
				varint_u<cutoff_width + 1> cutoff = out.slice<cutoff_index - 1, cutoff_width + 1>();

				bool all_zeros = ! ((bool) cutoff);
				bool all_ones = !  ((bool)~cutoff);
				if (!(all_zeros || all_ones))
					overflow[0] = true;
			}
		}

		// Cut off input MSBs
		real_out = out;
	}
	// Case (c) Output MSB further left
	//  i:     |====
	//  o:  |=======
	else {

		// Insert additional MSBs
		const int additional_width = MAX(out_msb - in_msb_adjusted, 0);

		// Case (c1) Sign modes are the same or UNSIGNED -> TWOSCOMPLEMENT
		if (a_sign_mode == r_sign_mode ||
		   (a_sign_mode == UNSIGNED && r_sign_mode == TWOSCOMPLEMENT))
		{
			// Sign extend. Can't overflow.
			real_out = out;

			const int msb_index = MAX(in_msb_adjusted - out_lsb + 1, 0);

			if (a_sign_mode == TWOSCOMPLEMENT)
				pad_msb<msb_index, a_sign_mode, r_sign_mode, additional_width>(real_out);
		}
		// Case (c1) TWOSCOMPLEMENT -> UNSIGNED
		else {

			// Overflow if input MSB is not 0
			if (hw_exception)
				overflow[0] =  (bool) out[out_msb];

			// Pad with 0s
			real_out = out;
		}
	}

	if (hw_exception)
		*exception = ExceptOverflow(overflow);

	return HWOffsetFix<r_size, r_offset, r_sign_mode>(real_out);
}

CAST_fixed2fixed(inline,HWEXCEPT,EXCEPTOVERFLOW) {
	const bool real_cast =
			r_size != a_size ||
			r_offset != a_offset ||
			r_sign_mode != a_sign_mode;

	return _cast_fixed2fixed<
		r_size, r_offset, r_sign_mode,round_mode,
		a_size, a_offset, a_sign_mode,hw_exception
	>(a, exception, Int2Type<real_cast>());
}


// Arithmetic ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <
	uint r_size, int r_offset, SignMode r_sign_mode,
	RoundMode round_mode, bool subtract,
	uint a_size, int a_offset, SignMode a_sign_mode,
	uint b_size, int b_offset, SignMode b_sign_mode
	HWEXCEPT
> inline
HWOffsetFix<r_size, r_offset, r_sign_mode>
_add_sub_fixed(	const HWOffsetFix<a_size, a_offset, a_sign_mode> &a, const HWOffsetFix<b_size, b_offset, b_sign_mode> &b, EXCEPTOVERFLOW ) {
	// round-to-nearest arithmetic rounds the operands *before* the operation
//	if (round_mode == TONEAR) {
//		if (hw_exception) {
//			ExceptOverflow exceptTruncA;
//			ExceptOverflow exceptTruncB;
//			const HWOffsetFix<r_size, r_offset, r_sign_mode> ra = cast_fixed2fixed<r_size, r_offset, r_sign_mode, TONEAR, a_size, a_offset, a_sign_mode, hw_exception>(a, &exceptTruncA);
//			const HWOffsetFix<r_size, r_offset, r_sign_mode> rb = cast_fixed2fixed<r_size, r_offset, r_sign_mode, TONEAR, b_size, b_offset, b_sign_mode, hw_exception>(b, &exceptTruncB);
//
//			ExceptOverflow exceptOverflow;
//			HWOffsetFix<r_size, r_offset, r_sign_mode> r = _add_sub_fixed <
//					r_size, r_offset, r_sign_mode, TRUNCATE, subtract,
//					r_size, r_offset, r_sign_mode,
//					r_size, r_offset, r_sign_mode,
//					hw_exception
//					> (ra, rb, &exceptOverflow);
//
//			*exception = ExceptOverflow(exceptOverflow.getBitString() | exceptTruncA.getBitString() | exceptTruncB.getBitString());
//			return r;
//		} else {
//			const HWOffsetFix<r_size, r_offset, r_sign_mode> ra = cast_fixed2fixed<r_size, r_offset, r_sign_mode, TONEAR>(a);
//			const HWOffsetFix<r_size, r_offset, r_sign_mode> rb = cast_fixed2fixed<r_size, r_offset, r_sign_mode, TONEAR>(b);
//
//			return _add_sub_fixed <
//					r_size, r_offset, r_sign_mode, TRUNCATE, subtract,
//					r_size, r_offset, r_sign_mode,
//					r_size, r_offset, r_sign_mode,
//					hw_exception
//					> (ra, rb, exception);
//		}
//	}

	const bool no_bit_growth =
			(r_size == a_size && r_size == b_size) &&
			(r_offset == a_offset && r_offset == b_offset) &&
			(r_sign_mode == a_sign_mode && r_sign_mode == b_sign_mode);

	if (no_bit_growth) {
		varint_u<r_size> r_bits = (subtract ? a.getBitString() - b.getBitString() : a.getBitString() + b.getBitString());

		if (hw_exception) {
			varint_u<1> overflow_bit(0);

			if (a_sign_mode == UNSIGNED) {
				if (subtract)
					overflow_bit[0] = r_bits > a.getBitString();
				else	// add
					overflow_bit[0] = r_bits < a.getBitString();
			}
			else {	// SIGNED
				bool nega = a.getBitString()[a_size-1];
				bool negb = b.getBitString()[b_size-1];
				bool negresult = r_bits[r_size-1];

				if (subtract)
					overflow_bit[0] = ( !nega && negb && negresult ) || ( nega && !negb && !negresult );
				else	// add
					overflow_bit[0] = ( !nega && !negb && negresult ) || ( nega && negb && !negresult );
			}

			*exception = ExceptOverflow(overflow_bit);
		}

		return HWOffsetFix<r_size, r_offset, r_sign_mode>(r_bits);
	}
	else {
		// Bit growth. Work out the ALL Bits, NO OVERFLOW type and use it as an intermediate type,
		// in case of subtraction, always force a sigend intermediate type
		ENCOMP(a_size, a_offset, a_sign_mode, b_size, b_offset, b_sign_mode, subtract);

		const int interm_size = encomp_size + 1;
		const int interm_offset = encomp_offset;
		const SignMode interm_sign_mode = encomp_sign_mode;

		varint_u<interm_size> a_extended =
				extend_input<interm_size, interm_offset, interm_sign_mode,
					a_size, a_offset, a_sign_mode>(a.getBitString());

		varint_u<interm_size> b_extended =
				extend_input<interm_size, interm_offset, interm_sign_mode,
					b_size, b_offset, b_sign_mode>(b.getBitString());

		// Now add/sub the extended values
		varint_u<interm_size> interm_bits = (subtract ? a_extended - b_extended : a_extended + b_extended);
		HWOffsetFix<interm_size, interm_offset, interm_sign_mode> interm_r(interm_bits);

		// Cast to the requested result type and return, HW Exceptions are handled by the caster
		if (hw_exception)
			return cast_fixed2fixed<r_size, r_offset, r_sign_mode, round_mode>(interm_r, exception);
		else
			return cast_fixed2fixed<r_size, r_offset, r_sign_mode, round_mode>(interm_r);
	}
}

ADD_fixed(inline,HWEXCEPT,EXCEPTOVERFLOW) {
	return _add_sub_fixed<
		r_size,r_offset,r_sign_mode,round_mode,false,
		a_size,a_offset,a_sign_mode,
		b_size,b_offset,b_sign_mode,
		hw_exception
	>(a, b, exception);
}

SUB_fixed(inline,HWEXCEPT,EXCEPTOVERFLOW) {
	return _add_sub_fixed<
		r_size,r_offset,r_sign_mode,round_mode,true,
		a_size,a_offset,a_sign_mode,
		b_size,b_offset,b_sign_mode,
		hw_exception
	>(a, b, exception);
}

NEG_fixed(,HWEXCEPT,EXCEPTOVERFLOW) {
	const bool no_bit_growth =
			r_size == a_size &&
			r_offset == a_offset &&
			r_sign_mode == a_sign_mode;

	if (no_bit_growth) {
		varint_u<r_size> r_bits = -a.getBitString();

		if (hw_exception) {
			varint_u<1> overflow_bit(0);

			// There is one case that can overflow in negation: the most negative number
			// (100...00) has no positive equivalent
			varint_u<a_size> most_neg_number;
			most_neg_number[a_size-1] = true;

			if (a.getBitString() == most_neg_number)
				overflow_bit[0] = true;

			*exception = ExceptOverflow(overflow_bit);
		}

		return HWOffsetFix<r_size, r_offset, r_sign_mode>(r_bits);
	}
	else {
		// Bit growth. Work out the ALL Bits, NO OVERFLOW type and use it as an intermediate type
		const int interm_size = a_size + 1;
		const int interm_offset = a_offset;
		const SignMode interm_sign_mode = TWOSCOMPLEMENT;

		varint_u<interm_size> a_extended =
				extend_input<interm_size, interm_offset, interm_sign_mode,
					a_size, a_offset, a_sign_mode>(a.getBitString());

		// Now negate the extended value
		varint_u<interm_size> interm_bits = -a_extended;
		HWOffsetFix<interm_size, interm_offset, interm_sign_mode> interm_r(interm_bits);

		// Cast to the requested result type and return, HW Exceptions are handled by the caster
		if (hw_exception)
			return cast_fixed2fixed<r_size, r_offset, r_sign_mode, round_mode>(interm_r, exception);
		else
			return cast_fixed2fixed<r_size, r_offset, r_sign_mode, round_mode>(interm_r);
	}
}

MUL_fixed(,HWEXCEPT,EXCEPTOVERFLOW) {
	// Multiplication always bit-grows + casts
	const int interm_size = a_size + b_size + ((a_sign_mode == b_sign_mode) ? 0 : 1);
	const SignMode interm_sign_mode = (a_sign_mode == b_sign_mode) ? a_sign_mode : TWOSCOMPLEMENT;

	varint_u<interm_size> a_extended =
			extend_input<interm_size, a_offset, interm_sign_mode,
				a_size, a_offset, a_sign_mode>(a.getBitString());

	varint_u<interm_size> b_extended =
			extend_input<interm_size, b_offset, interm_sign_mode,
				b_size, b_offset, b_sign_mode>(b.getBitString());

	// Now multiply the extended value
	varint_u<interm_size> interm_bits = a_extended * b_extended;
	HWOffsetFix<interm_size, a_offset+b_offset, interm_sign_mode> interm_r(interm_bits);

	// Cast to the requested result type and return, HW Exceptions are handled by the caster
	if (hw_exception)
		return cast_fixed2fixed<r_size, r_offset, r_sign_mode, round_mode>(interm_r, exception);
	else
		return cast_fixed2fixed<r_size, r_offset, r_sign_mode, round_mode>(interm_r);
}

DIV_fixed(,HWEXCEPT,EXCEPTFLOAT) {
	// Layer 1 (MaxDCFixedDiv.java): Pre-process inputs

	// Pre-process inputs //////////////////////////////////////////////////

	bool a_sgn = (a_sign_mode == TWOSCOMPLEMENT) ? a.m_bits[a_size-1] : false;
	bool b_sgn = (b_sign_mode == TWOSCOMPLEMENT) ? b.m_bits[b_size-1] : false;
	bool r_sgn = a_sgn ^ b_sgn;

	const varint_u<a_size> a_uns = (a_sign_mode == TWOSCOMPLEMENT)
			? (a_sgn ? -a.getBitString() : a.getBitString())
			: a.getBitString();

	const varint_u<b_size> b_uns = (b_sign_mode == TWOSCOMPLEMENT)
			? (b_sgn ? -b.getBitString() : b.getBitString())
			: b.getBitString();

	// Instantiate the unsigned divider ////////////////////////////////////

	bool overflow = false;
	bool round_bit = false;
	bool sticky_bit = false;

	varint_u<r_size> result = _div_unsigned<r_size, r_offset, round_mode,
			a_size, a_offset, b_size, b_offset, hw_exception>(
					a_uns,
					b_uns,
					(hw_exception ? &overflow : NULL),
					&round_bit,
					&sticky_bit);

	// Post-process outputs ////////////////////////////////////////////////

	if (r_sgn) {
		// Check for overflow caused by negation
		varint_s<r_size+1> result_extended(result);
		result_extended = -result_extended;

		// Fix rounding
		if (round_mode == TRUNCATE && sticky_bit) {
			// Round to -inf
			result_extended--;
		}
		else if (round_mode == TONEAR && round_bit && !sticky_bit) {
			// Round to 0 if remainder is 0.5
			result_extended++;
		}

		if (result_extended.get_bit(r_size-1) != result_extended.get_bit(r_size))
			overflow = true;

		result = result_extended.slice<0, r_size>();
	}
	else if (r_sign_mode != UNSIGNED) {
		if (result.get_bit(r_size - 1) == 1)
			overflow = true;
	}

	if (hw_exception) {
		if (r_sign_mode == UNSIGNED)
			overflow |= r_sgn;

		bool b_is_zero = !((bool)b_uns);

		// Output HW Exception (mask out overflow in div/0 case)
		varint_u<4> except_vector(0);
		except_vector[1] = overflow && !b_is_zero;
		except_vector[3] = b_is_zero;

		*exception = ExceptFloat(except_vector);
	}

	return HWOffsetFix<r_size,r_offset,r_sign_mode>(result);
}

DIVMOD_fixed(,HWEXCEPT,EXCEPTFLOAT) {
	const int num_bits = n_size;
	const int denom_bits = d_size;
	const int skip_bits = n_size - q_size;
	const int frac_bits = 0;
	const bool get_remainder = true;

	bool overflow = false;

	varint_u<num_bits> num = n.getBitString();
	varint_u<denom_bits> denom = d.getBitString();

	varint_u<denom_bits> rem(0);
	varint_u<q_size> quot = _div_integer<num_bits, denom_bits, skip_bits,
			frac_bits, get_remainder, hw_exception>(num, denom, &rem, (hw_exception ? &overflow : NULL));

	*remainder = HWOffsetFix<d_size,d_offset,d_sign_mode>(rem);

	if (hw_exception) {
		bool b_is_zero = !((bool)denom);

		// Output HW Exception (mask out overflow in div/0 case)
		varint_u<4> except_vector(0);
		except_vector[1] = overflow && !b_is_zero;
		except_vector[3] = b_is_zero;

		*exception = ExceptFloat(except_vector);
	}

	return HWOffsetFix<q_size,q_offset,q_sign_mode>(quot);
}

// Comparison ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <_CompareOp op, typename T> inline
HWBool _cmp_types(const T& a, const T& b) {
	switch(op) {
		case EQ:  return const_bool( a == b );
		case NEQ: return const_bool( a != b );
		case GT:  return const_bool( a >  b );
		case GTE: return const_bool( a >= b );
		case LT:  return const_bool( a <  b );
		case LTE: return const_bool( a <= b );
	}
}

template <	_CompareOp op, uint a_size, int a_offset, SignMode a_sign_mode,
			uint b_size, int b_offset, SignMode b_sign_mode> inline
HWBool _cmp_fixed(	const HWOffsetFix<a_size, a_offset, a_sign_mode> &a,
					const HWOffsetFix<b_size, b_offset, b_sign_mode> &b,
					Int2Type<false>, Int2Type<false>)
{
	return _cmp_types<op>(a.getBitString(), b.getBitString());
}

template <	_CompareOp op, uint a_size, int a_offset, SignMode a_sign_mode,
			uint b_size, int b_offset, SignMode b_sign_mode> inline
HWBool _cmp_fixed(	const HWOffsetFix<a_size, a_offset, a_sign_mode> &a,
					const HWOffsetFix<b_size, b_offset, b_sign_mode> &b,
					Int2Type<false> extend_inputs, Int2Type<true> signed_inputs)
{
	return _cmp_types<op>(varint_s<a_size>(a.getBitString()), varint_s<b_size>(b.getBitString()));
}

template <	_CompareOp op, uint a_size, int a_offset, SignMode a_sign_mode,
			uint b_size, int b_offset, SignMode b_sign_mode> inline
HWBool _cmp_fixed(	const HWOffsetFix<a_size, a_offset, a_sign_mode> &a,
					const HWOffsetFix<b_size, b_offset, b_sign_mode> &b,
					Int2Type<true> extend_inputs, Int2Type<false> signed_inputs)
{
	ENCOMP(a_size, a_offset, a_sign_mode, b_size, b_offset, b_sign_mode, false);

	varint_u<encomp_size> a_extended =
			extend_input<encomp_size, encomp_offset, encomp_sign_mode,
				a_size, a_offset, a_sign_mode>(a.getBitString());

	varint_u<encomp_size> b_extended =
			extend_input<encomp_size, encomp_offset, encomp_sign_mode,
				b_size, b_offset, b_sign_mode>(b.getBitString());

	return _cmp_types<op>(a_extended, b_extended);
}

template <	_CompareOp op, uint a_size, int a_offset, SignMode a_sign_mode,
			uint b_size, int b_offset, SignMode b_sign_mode> inline
HWBool _cmp_fixed(	const HWOffsetFix<a_size, a_offset, a_sign_mode> &a,
					const HWOffsetFix<b_size, b_offset, b_sign_mode> &b,
					Int2Type<true> extend_inputs, Int2Type<true> signed_inputs)
{
	ENCOMP(a_size, a_offset, a_sign_mode, b_size, b_offset, b_sign_mode, false);

	varint_s<encomp_size> a_extended =
			extend_input<encomp_size, encomp_offset, encomp_sign_mode,
				a_size, a_offset, a_sign_mode>(a.getBitString());

	varint_s<encomp_size> b_extended =
			extend_input<encomp_size, encomp_offset, encomp_sign_mode,
				b_size, b_offset, b_sign_mode>(b.getBitString());

	return _cmp_types<op>(a_extended, b_extended);
}

#define _CMP_FIXED(op)																						\
op##_fixed(inline) {																						\
	const bool inputs_no_match = a_size != b_size || a_offset != b_offset || a_sign_mode != b_sign_mode;	\
	const bool compare_signed = a_sign_mode == TWOSCOMPLEMENT || b_sign_mode == TWOSCOMPLEMENT;				\
	return _cmp_fixed<op>(a, b, Int2Type<inputs_no_match>(), Int2Type<compare_signed>());					\
}

_CMP_FIXED(EQ)
_CMP_FIXED(NEQ)
_CMP_FIXED(GT)
_CMP_FIXED(GTE)
_CMP_FIXED(LT)
_CMP_FIXED(LTE)

#undef _CMP_FIXED

// Shifting //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SHIFTLEFT_fixed(inline,HWEXCEPT,EXCEPTOVERFLOW) {
	varint_u<a_size> shifted = a.getBitString() << shift_by;

	if (hw_exception) {
		// Detect shift exceptions by looking to see whether we lost any meaningful bits
		// Only sequences it is valid to lose are 00...000 and 11....11, including the leftmost
		// remaining bit. This assures that no sign change occurs.
		varint_u<1> overflow_bit(0);

		// We right-shift arithmetic to work out the cut-off bits. If they are all 1s
		// or all 0s we're fine
		varint_s<a_size> cutoff = a.getBitString();
		cutoff >>= (a_size - (shift_by+1));

		bool all_zeros = !((bool)cutoff);
		bool all_ones = !((bool)~cutoff);

		if (! (all_zeros || all_ones))
			overflow_bit[0] = true;

		*exception = ExceptOverflow(overflow_bit);
	}

	return HWRawBits<a_size>(shifted);
}

SHIFTRIGHT_fixed(inline) {
	if (a_sign_mode == UNSIGNED)
		return HWRawBits<a_size>((varint_u<a_size>)(a.getBitString() >> shift_by));
	else // TWOSCOMPLEMENT
		return HWRawBits<a_size>((varint_u<a_size>)((varint_s<a_size>)(varint_s<a_size>(a.getBitString()) >> shift_by)));
}

// Bit-wise operations ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

NOT_fixed(inline) {
	return HWOffsetFix<r_size,r_offset,r_sign_mode>( ~a.getBitString() );
}

AND_fixed(inline) {
	return HWOffsetFix<r_size,r_offset,r_sign_mode>( a.getBitString() & b.getBitString() );
}

OR_fixed(inline) {
	return HWOffsetFix<r_size,r_offset,r_sign_mode>( a.getBitString() | b.getBitString() );
}

XOR_fixed(inline) {
	return HWOffsetFix<r_size,r_offset,r_sign_mode>( a.getBitString() ^ b.getBitString() );
}


} /* end namespace maxcompilersim */

#undef MAX /* end Helper Macros */
#undef MIN
#undef ENCOMP

#endif /* HWOFFSETFIX_IMPL_H_ */
