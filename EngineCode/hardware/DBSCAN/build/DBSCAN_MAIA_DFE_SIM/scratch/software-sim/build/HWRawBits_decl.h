#ifndef HWRAWBITS_DECL_H_
#define HWRAWBITS_DECL_H_

#include <sstream>
#include <iostream>

#include "HWTypes.h"

namespace maxcompilersim {

CLASS_HWRawBits {
public:
	enum { totalBitSize = size };

	// Constructors
	HWRawBits(varint_u<size> bits);
	HWRawBits(long v);
	HWRawBits();

	// Getters
	varint_u<size> getBitString() const;
	long getValueAsLong() const;
	bool getValueAsBool() const;

	friend std::ostream& operator<<(std::ostream& o, const HWRawBits<size>& v) {
		return o << v.getBitString();
	}

	// Report type
	std::string getTypeString() const {
		std::ostringstream tmp;
		tmp << "{HWRawBits:" << size << "}";
		return tmp.str();
	}

	// Shifting
	SHIFTLEFT_bits(friend);
	SHIFTRIGHT_bits(friend);

	// Operators
	NOT_bits(friend);
	AND_bits(friend);
	OR_bits(friend);
	XOR_bits(friend);
	EQ_bits(friend);
	NEQ_bits(friend);

	// Casting
	CAST_bits2bits(friend);
	CAST_bits2float(friend);
	CAST_float2bits(friend);
	CAST_bits2fixed(friend);
	CAST_fixed2bits(friend);

	// Cat-ting
	CAT_bits_bits(friend);
	CAT_bits_float(friend);
	CAT_bits_fixed(friend);
	CAT_float_bits(friend);
	CAT_float_float(friend);
	CAT_float_fixed(friend);
	CAT_fixed_bits(friend);
	CAT_fixed_float(friend);
	CAT_fixed_fixed(friend);

	// Slice-ing
	SLICE_bits(friend);
	SLICE_float(friend);
	SLICE_fixed(friend);

private:
	varint_u<size> m_bits;
};

} /* end namespace maxcompilersim */

#endif /* HWRAWBITS_DECL_H_ */
