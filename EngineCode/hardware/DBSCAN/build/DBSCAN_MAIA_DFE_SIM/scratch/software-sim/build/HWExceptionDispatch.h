#ifndef HWEXCEPTIONDISPATCH_H_
#define HWEXCEPTIONDISPATCH_H_

#include "HWTypes.h"

namespace maxcompilersim {

// Casts /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CAST_float2float(inline,) {
	return cast_float2float<r_esize,r_msize,a_esize,a_msize,false>(a,NULL);
}

CAST_float2float(inline,,EXCEPTFLOAT) {
	return cast_float2float<r_esize,r_msize,a_esize,a_msize,true>(a,exception);
}

CAST_float2fixed(inline,) {
	return cast_float2fixed<r_size,r_offset,r_sign_mode,a_esize,a_msize,false>(a,NULL);
}

CAST_float2fixed(inline,,EXCEPTFLOAT) {
	return cast_float2fixed<r_size,r_offset,r_sign_mode,a_esize,a_msize,true>(a,exception);
}

CAST_fixed2float(inline,) {
	return cast_fixed2float<r_esize,r_msize,a_size,a_offset,a_sign_mode,false>(a,NULL);
}

CAST_fixed2float(inline,,EXCEPTFLOAT) {
	return cast_fixed2float<r_esize,r_msize,a_size,a_offset,a_sign_mode,true>(a,exception);
}


CAST_fixed2fixed(inline,) {
	return cast_fixed2fixed<r_size,r_offset,r_sign_mode,round_mode,a_size,a_offset,a_sign_mode,false>(a,NULL);
}

CAST_fixed2fixed(inline,,EXCEPTOVERFLOW) {
	return cast_fixed2fixed<r_size,r_offset,r_sign_mode,round_mode,a_size,a_offset,a_sign_mode,true>(a,exception);
}


// Floating point arithmetic /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ADD_float(inline,) {
	return add_float<r_esize,r_msize,false>(a,b,NULL);
}

ADD_float(inline,,EXCEPTFLOAT) {
	return add_float<r_esize,r_msize,true>(a,b,exception);
}

SUB_float(inline,) {
	return sub_float<r_esize,r_msize,false>(a,b,NULL);
}

SUB_float(inline,,EXCEPTFLOAT) {
	return sub_float<r_esize,r_msize,true>(a,b,exception);
}

MUL_float(inline,) {
	return mul_float<r_esize,r_msize,false>(a,b,NULL);
}

MUL_float(inline,,EXCEPTFLOAT) {
	return mul_float<r_esize,r_msize,true>(a,b,exception);
}

DIV_float(inline,) {
	return div_float<r_esize,r_msize,false>(a,b,NULL);
}

DIV_float(inline,,EXCEPTFLOAT) {
	return div_float<r_esize,r_msize,true>(a,b,exception);
}

SQRT_float(inline,) {
	return sqrt_float<r_esize,r_msize,false>(a,NULL);
}

SQRT_float(inline,,EXCEPTFLOAT) {
	return sqrt_float<r_esize,r_msize,true>(a,exception);
}


// Fixed point arithmetic ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ADD_fixed(inline,) {
	return add_fixed<r_size,r_offset,r_sign_mode,round_mode,a_size,a_offset,a_sign_mode,b_size,b_offset,b_sign_mode,false>(a,b,NULL);
}

ADD_fixed(inline,,EXCEPTOVERFLOW) {
	return add_fixed<r_size,r_offset,r_sign_mode,round_mode,a_size,a_offset,a_sign_mode,b_size,b_offset,b_sign_mode,true>(a,b,exception);
}

SUB_fixed(inline,) {
	return sub_fixed<r_size,r_offset,r_sign_mode,round_mode,a_size,a_offset,a_sign_mode,b_size,b_offset,b_sign_mode,false>(a,b,NULL);
}

SUB_fixed(inline,,EXCEPTOVERFLOW) {
	return sub_fixed<r_size,r_offset,r_sign_mode,round_mode,a_size,a_offset,a_sign_mode,b_size,b_offset,b_sign_mode,true>(a,b,exception);
}

MUL_fixed(inline,) {
	return mul_fixed<r_size,r_offset,r_sign_mode,round_mode,a_size,a_offset,a_sign_mode,b_size,b_offset,b_sign_mode,false>(a,b,NULL);
}

MUL_fixed(inline,,EXCEPTOVERFLOW) {
	return mul_fixed<r_size,r_offset,r_sign_mode,round_mode,a_size,a_offset,a_sign_mode,b_size,b_offset,b_sign_mode,true>(a,b,exception);
}

DIV_fixed(inline,) {
	return div_fixed<r_size,r_offset,r_sign_mode,round_mode,a_size,a_offset,a_sign_mode,b_size,b_offset,b_sign_mode,false>(a,b,NULL);
}

DIV_fixed(inline,,EXCEPTFLOAT) {
	return div_fixed<r_size,r_offset,r_sign_mode,round_mode,a_size,a_offset,a_sign_mode,b_size,b_offset,b_sign_mode,true>(a,b,exception);
}

DIVMOD_fixed(inline,) {
	return divmod_fixed<q_size,q_offset,q_sign_mode,n_size,n_offset,n_sign_mode,d_size,d_offset,d_sign_mode,false>(n,d,remainder,NULL);
}

DIVMOD_fixed(inline,,EXCEPTFLOAT) {
	return divmod_fixed<q_size,q_offset,q_sign_mode,n_size,n_offset,n_sign_mode,d_size,d_offset,d_sign_mode,true>(n,d,remainder,exception);
}

NEG_fixed(inline,) {
	return neg_fixed<r_size,r_offset,r_sign_mode,round_mode,a_size,a_offset,a_sign_mode,false>(a,NULL);
}

NEG_fixed(inline,,EXCEPTOVERFLOW) {
	return neg_fixed<r_size,r_offset,r_sign_mode,round_mode,a_size,a_offset,a_sign_mode,true>(a,exception);
}

SHIFTLEFT_fixed(inline,) {
	return shift_left_fixed<a_size,a_offset,a_sign_mode,false>(a,shift_by,NULL);
}

SHIFTLEFT_fixed(inline,,EXCEPTOVERFLOW) {
	return shift_left_fixed<a_size,a_offset,a_sign_mode,true>(a,shift_by,exception);
}

} /* end namespace maxcompilersim */

#endif /* HWEXCEPTIONDISPATCH_H_ */
