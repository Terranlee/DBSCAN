#ifndef HWRAWBITS_IMPL_H_
#define HWRAWBITS_IMPL_H_

#include "HWRawBits_decl.h"

namespace maxcompilersim {

// Constructors //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <uint size> inline
HWRawBits<size>::HWRawBits(varint_u<size> bits)
	: m_bits(bits)
	{ }

template <uint size> inline
HWRawBits<size>::HWRawBits(long v)
	: m_bits(varint_u<size>(v))
	{ }

template <uint size> inline
HWRawBits<size>::HWRawBits()
	// TODO Uninitialized value
	{ }

// Getters ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <uint size> inline
varint_u<size> HWRawBits<size>::getBitString() const {
	return m_bits;
}

template <uint size> inline
long HWRawBits<size>::getValueAsLong() const {
	return (long)m_bits;
}

template <uint size> inline
bool HWRawBits<size>::getValueAsBool() const {
	return (bool)m_bits;
}

// Casting ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CAST_bits2bits(inline) {
	varint_u<r_size> resized = a.getBitString();
	return HWRawBits<r_size>(resized);
}

// Reinterpret Casting ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CAST_float2bits(inline) {
	return HWRawBits<a_esize+a_msize>( a.getBitString() );
}

CAST_bits2float(inline) {
	return HWFloat<r_esize, r_msize>( a.getBitString() );
}

CAST_fixed2bits(inline) {
	return HWRawBits<a_size>( a.getBitString() );
}

CAST_bits2fixed(inline) {
	return HWOffsetFix<r_size,r_offset,r_sign_mode>( a.getBitString() );
}

// Shifting //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SHIFTLEFT_bits(inline) {
	return HWRawBits<a_size>((varint_u<a_size>)(a.getBitString() << shift_by));
}

SHIFTRIGHT_bits(inline) {
	return HWRawBits<a_size>((varint_u<a_size>)(a.getBitString() >> shift_by));
}

// Bit-wise operations ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

NOT_bits(inline) {
	return HWRawBits<r_size>( ~a.getBitString() );
}

AND_bits(inline) {
	return HWRawBits<r_size>( a.getBitString() & b.getBitString() );
}

OR_bits(inline) {
	return HWRawBits<r_size>( a.getBitString() | b.getBitString() );
}

XOR_bits(inline) {
	return HWRawBits<r_size>( a.getBitString() ^ b.getBitString() );
}

EQ_bits(inline) {
	return HWBool( a.getBitString() == b.getBitString() );
}

NEQ_bits(inline) {
	return HWBool( a.getBitString() != b.getBitString() );
}

// Cat-ting //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define CATTER(name,asize,bsize) 								\
name(inline) {													\
	const uint r_size = asize + bsize;							\
	varint_u<r_size> a_large = a.getBitString();				\
	varint_u<r_size> b_large = b.getBitString();				\
	varint_u<r_size> result = (a_large << bsize) | b_large;		\
	return HWRawBits<r_size>(result);							\
}

CATTER(CAT_bits_bits,	a_size, b_size)
CATTER(CAT_bits_float,	a_size, b_esize+b_msize)
CATTER(CAT_bits_fixed,	a_size, b_size)
CATTER(CAT_float_bits,	a_esize+a_msize, b_size)
CATTER(CAT_float_float,	a_esize+a_msize, b_esize+b_msize)
CATTER(CAT_float_fixed, a_esize+a_msize, b_size)
CATTER(CAT_fixed_bits,	a_size, b_size)
CATTER(CAT_fixed_float,	a_size, b_esize+b_msize)
CATTER(CAT_fixed_fixed,	a_size, b_size)

#undef CATTER

// Slice-ing /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SLICE_bits(inline) {
	varint_u<a_size> shifted = a.getBitString() >> base;
	return HWRawBits<length>(varint_u<length>(shifted));
}

SLICE_float(inline) {
	varint_u<a_esize+a_msize> shifted = a.getBitString() >> base;
	return HWRawBits<length>(varint_u<length>(shifted));
}

SLICE_fixed(inline) {
	varint_u<a_size> shifted = a.getBitString() >> base;
	return HWRawBits<length>(varint_u<length>(shifted));
}

} /* end namespace maxcompilersim */

#endif /* HWRAWBITS_IMPL_H_ */
