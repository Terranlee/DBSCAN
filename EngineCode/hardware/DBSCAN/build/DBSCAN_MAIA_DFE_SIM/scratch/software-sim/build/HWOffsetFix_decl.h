#ifndef HWOFFSETFIX_DECL_H_
#define HWOFFSETFIX_DECL_H_

#include <sstream>
#include <iostream>

#include "HWTypes.h"

namespace maxcompilersim {

CLASS_HWOffsetFix {
public:
	enum { totalBitSize = size };

	// Constructors
	HWOffsetFix(varint_u<size> raw_bits);
	HWOffsetFix(double v);
	HWOffsetFix(long v);
	HWOffsetFix(int v) { *this = HWOffsetFix(long(v)); }
	HWOffsetFix();

	// Getters
	varint_u<size> getBitString() const;
	double getValueAsDouble() const;
	long getValueAsLong() const;
	bool getValueAsBool() const;

	friend std::ostream& operator<<(std::ostream& o, const HWOffsetFix<size,offset,sign_mode>& v) {
		if(o.flags() & std::ios_base::dec) {
			if (offset == 0) {
				// ordinary integer
				if(sign_mode == TWOSCOMPLEMENT)
					return o << varint_s<size>(v.getBitString());
				else
					return o << v.getBitString();

			} else {
				return o << v.getValueAsDouble();
			}
		} else {
			// hex and binary (%o)
			return o << v.getBitString();
		}
	}

	// Report type
	std::string getTypeString() const {
		std::ostringstream tmp;
		tmp << "{HWOffsetFix:" << size << ", " << offset << ", "
			<< (sign_mode == UNSIGNED ? "UNSIGNED" : "TWOSCOMPLEMENT") << "}";
		return tmp.str();
	}

	// Constants
	CONST_true(friend);
	CONST_false(friend);
	CONST_bool(friend);

	// Operators
	ADD_fixed(friend,HWEXCEPT,EXCEPTOVERFLOW);
	SUB_fixed(friend,HWEXCEPT,EXCEPTOVERFLOW);
	MUL_fixed(friend,HWEXCEPT,EXCEPTOVERFLOW);
	DIV_fixed(friend,HWEXCEPT,EXCEPTFLOAT);
	DIVMOD_fixed(friend,HWEXCEPT,EXCEPTFLOAT);
	NEG_fixed(friend,HWEXCEPT,EXCEPTOVERFLOW);

	// Comparison
	EQ_fixed(friend);
	NEQ_fixed(friend);
	GT_fixed(friend);
	GTE_fixed(friend);
	LT_fixed(friend);
	LTE_fixed(friend);

	// Shifting
	SHIFTLEFT_fixed(friend,HWEXCEPT,EXCEPTOVERFLOW);
	SHIFTRIGHT_fixed(friend);

	// Bitwise Operators
	NOT_fixed(friend);
	AND_fixed(friend);
	OR_fixed(friend);
	XOR_fixed(friend);

	// Casting
	CAST_fixed2fixed(friend,HWEXCEPT,EXCEPTOVERFLOW);

private:
	varint_u<size> m_bits;

};

} /* end namespace maxcompilersim */

#endif /* HWOFFSETFIX_DECL_H_ */
