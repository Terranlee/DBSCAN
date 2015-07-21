/**
 * @file	varint_s.h
 * @author  Stefan Bach <stefan@maxeler.com>
 *
 * @section DESCRIPTION
 *
 * A templated arbitrary size signed integer. This implementation provides
 * overloaded operators and is meant to exhibit identical behavior to signed
 * char, int, long, etc. types.
 * \li Standard arithmetic, comparison and bitwise operators are provided,
 *  with the exception of the division and modulo operation.
 * \li Only types of identical bit width can be combined in a binary operation.
 * \li Implicit casting between different bit widths is provided for assignment
 *  and copy construction. This can be used to resize operands of different bit
 *  widths for binary operations.
 * \li Chunks of up to 64 raw bits can be read/written through the get_bits and
 *  set_bits method. Single bit access is provided through get_big, set_bit and
 *  the subscript operator.
 */

#ifndef VARINT_S_H_
#define VARINT_S_H_

#include "HWTypes.h"

namespace maxcompilersim {

CLASS_varint_s {
private:
	friend class varint_u<size>;

	// We wrap around an unsigned number and clean up after it
	varint_u<size> m_ustore;

public:
	/**
	 * Construct a new signed integer of size bits and set it to 0.
	 */
	varint_s()
		: m_ustore()
		{ }

	/**
	 * Construct a new signed integer of size bits and initialize the
	 * lowest of min(64, size) bits with v
	 */
	varint_s(uint64_t v)
		: m_ustore(v)
		{ }

	/**
	 * Construct a new signed integer of size bits and initialize the
	 * lowest of min(32, size) bits with v
	 */
	varint_s(uint32_t v)
		: m_ustore(uint64_t(v))
	{
		// Because we now provide signed and unsigned constructors, they
		// will be ambiguous if the bit width doesn't exactly match.
		// We thus provide them both in 32 and 64 bit.
	}

	/**
	 * Construct a new signed integer of size bits and initialize it
	 * with the signed number v
	 */
	varint_s(int64_t v)
		: m_ustore(v)
	{
		// If v is negative and size > 64 we need to set the higher
		// order bits to 1. Otherwise we're fine.
		if (v < 0 && size > 64)
			for (uint bit_num=64; bit_num<size; bit_num+=64)
				m_ustore.set_bits(bit_num, 64, uint64_t(int64_t(-1)));
	}


	/**
	 * Construct a new signed integer of size bits and initialize it
	 * with the signed number v
	 */
	varint_s(int32_t v)
		: m_ustore(uint64_t(int64_t(v)))
	{
		// If v is negative and size > 64 we need to set the higher
		// order bits to 1. Otherwise we're fine.
		if (v < 0 && size > 64)
			for (uint bit_num=64; bit_num<size; bit_num+=64)
				m_ustore.set_bits(bit_num, 64, uint64_t(int64_t(-1)));
	}

	/**
	 * Construct a new unsigned integer of size bits and initialize
	 * bits offset...min(offset+s_chunk_bits, size) with v.
	 */
	varint_s(uint64_t v, uint offset)
		: m_ustore(v, offset)
		{ }

//	(Not used?)
//	/**
//	 * Construct a new unsigned integer of size bits and initialize the
//	 * lowest of min(s_num_chunks, avail_chunks) chunks with v[].
//	 */
//	varint_s(const uint64_t v[], uint avail_chunks)
//		: m_ustore(v, avail_chunks)
//		{ }

	/**
	 * Construct a new signed integer of size bits and initialize the
	 * lowest of min(size, size2) bits with v. Casting between signed/
	 * unsigned and different sizes is supported through this constructor.
	 */
	template <uint size2>
	varint_s(const varint_u<size2>& v)
		: m_ustore(v)
		{ }

	/**
	 * Construct a new signed integer of size bits and initialize the
	 * lowest of min(size, size2) bits with v, expand the sign if size
	 * is larger than size2. Casting between different sizes is supported
	 * through this constructor.
	 */
	template <uint size2>
	varint_s(const varint_s<size2>& v)
		: m_ustore( varint_u<size2>(v) )
	{
		// If v is negative and size > size2 we need to set the higher
		// order bits to 1. Otherwise we're fine.
		if (v[size2-1] && size > size2)
			for (uint bit_num=size2; bit_num<size; bit_num+=64)
				m_ustore.set_bits(bit_num, 64, uint64_t(int64_t(-1)));
	}

	/**
	 * Assign the lowest of min(size, size2) bits with v, set other bits
	 * to 0. Casting between signed/unsigned and different sizes is
	 * supported through this assignment operator.
	 */
	template <uint size2>
	varint_s<size>& operator=(const varint_u<size2>& v) {
		m_ustore = v;
		return *this;
	}

	/**
	 * Assign the lowest of min(size, size2) bits with v, expand the sign if size
	 * is larger than size2. Casting between different sizes is supported
	 * through this assignment operator.
	 */
	template <uint size2>
	varint_s<size>& operator=(const varint_s<size2>& v) {
		m_ustore = varint_u<size2>(v);

		// If v is negative and size > size2 we need to set the higher
		// order bits to 1. Otherwise we're fine.
		if (v[size2-1] && size > size2)
			for (uint bit_num=size2; bit_num<size; bit_num+=64)
				m_ustore.set_bits(bit_num, 64, uint64_t(int64_t(-1)));

		return *this;
	}

	// Casting and I/O /////////////////////////////////////////////////////////

	operator bool() const
	{ return bool(m_ustore); }


	int64_t toInt64() const {
		if (size >= 64)
			return m_ustore.toInt64();				// Cut off extra bits
		else
			return varint_s<64>(*this).toInt64();	// Sign extend
	}
	operator signed char()	const { return static_cast<signed char>(toInt64()); }
	operator short int()	const { return static_cast<short int>(toInt64()); }
	operator int()			const { return static_cast<int>(toInt64()); }
	operator long int()		const { return static_cast<long int>(toInt64()); }

	uint64_t toUInt64()				const {	return m_ustore.toUInt64();	}
	operator unsigned char()		const { return static_cast<unsigned char>(toUInt64()); }
	operator unsigned short int()	const { return static_cast<unsigned short int>(toUInt64()); }
	operator unsigned int()			const { return static_cast<unsigned int>(toUInt64()); }
	operator unsigned long int()	const { return static_cast<unsigned long int>(toUInt64()); }


	friend std::ostream& operator<<(std::ostream& o, const varint_s<size> &v) {
		if(o.flags() & std::ios_base::dec) {
			bool showpos = o.flags() & std::ios_base::showpos;
			o.unsetf(std::ios_base::showpos);

			if(v.get_bit(size-1)) { // is negative?
				o << "-" << (-v).m_ustore;
			} else {
				if(showpos) {
					o << "+";
					o.unsetf(std::ios_base::showpos);
				}

				o << v.m_ustore;
			}

			if(showpos)
				o.setf(std::ios_base::showpos);

			return o;
		} else {
			return o << v.m_ustore;
		}
	}


	// Bit access //////////////////////////////////////////////////////////////

	// Non-checking helper method for copy/assignment operators
	uint64_t get_chunk(uint idx) const
	{ return m_ustore.get_chunk(idx); }

	// Non-checking helper method for multiplication
	uint64_t get_half_chunk(uint idx) const
	{ return m_ustore.get_half_chunk(idx); }

	uint64_t get_bits(int base, int length) const {
		uint64_t result = m_ustore.get_bits(base, length);

		// Bit hack to extend sign from a variable position
		// http://graphics.stanford.edu/~seander/bithacks.html#VariableSignExtend
		if(base+length >= size) {
			int const m = 1U << (size - base - 1);
			result = (result ^ m) - m;
		}
		return result;
	}

	template<uint base, uint length, uint size2>
	void set_bits(varint_s<size2> value)
	{ m_ustore.set_bits<base,length,size2>(varint_u<size2>(value)); }

	template<uint base, uint length, uint size2>
	void set_bits(varint_u<size2> value)
	{ m_ustore.set_bits<base,length,size2>(value); }

	void set_bits(int base, int length, uint64_t value)
	{ m_ustore.set_bits(base, length, value); }

	bool get_bit(uint base) const {
		// Sign extend to the left if base is out of bounds
		if (base >= size)
			return m_ustore.get_bit(size-1);
		else
			return m_ustore.get_bit(base);
	}

	void set_bit(int base, bool value)
	{ m_ustore.set_bit(base, value); }

	class bit_inspector {
	private:
		friend class varint_s<size>;
		const varint_s<size>& m_varint;
		uint m_bit;
		bit_inspector(const varint_s<size>& varint, uint bit)
			: m_varint(varint), m_bit(bit) {}
	public:
		operator bool() const { return m_varint.get_bit(m_bit); }
	};
	const bit_inspector operator[] (unsigned idx) const {
		return bit_inspector(*this, idx);
	}

	class bit_setter {
	private:
		friend class varint_s<size>;
		varint_s<size>& m_varint;
		uint m_bit;
		bit_setter(varint_s<size>& varint, uint bit)
			: m_varint(varint), m_bit(bit) {}
	public:
		operator bool() const { return m_varint.get_bit(m_bit); }
		bool operator= (bool value) { m_varint.set_bit(m_bit, value); return value; }
	};
	bit_setter operator[] (unsigned idx) {
		return bit_setter(*this, idx);
	}

	// Arithmetic operators ////////////////////////////////////////////////////

	const varint_s<size> operator+() const {
		return *this;
	}

	const varint_s<size> operator-() const {
		varint_s result = *this;
		result.m_ustore = -result.m_ustore;
		return result;
	}

	varint_s<size>& operator+=(const varint_s<size>& rhs) {
		m_ustore += rhs.m_ustore;
		return *this;
	}
	const varint_s<size> operator+(const varint_s<size>& rhs) const {
		return (varint_s(*this) += rhs);
	}

	varint_s<size>& operator-=(const varint_s<size>& rhs) {
		return *this += (-rhs);
	}
	const varint_s<size> operator-(const varint_s<size>& rhs) const {
		return (varint_s(*this) -= rhs);
	}

	const varint_s<size> operator*(const varint_s<size>& rhs) const {
		varint_s result = *this;
		result.m_ustore = m_ustore * rhs.m_ustore;
		return result;
	}
	varint_s<size>& operator*=(const varint_s<size>& rhs) {
		varint_s<size> tmp = (*this * rhs);
		(*this) = tmp;
		return *this;
	}

//	const varint_s<size> operator/(const varint_s<size>& rhs) const {
//		COMPILER_CHECK(false, VarintSigned_Division_not_implemented);
//	}
//	varint_s<size>& operator/=(const varint_s<size>& rhs) {
//		COMPILER_CHECK(false, VarintSigned_Division_not_implemented);
//	}
//	const varint_s<size> operator%(const varint_s<size>& rhs) const {
//		COMPILER_CHECK(false, VarintSigned_Modulo_not_implemented);
//	}
//	varint_s<size>& operator%=(const varint_s<size>& rhs) {
//		COMPILER_CHECK(false, VarintSigned_Modulo_not_implemented);
//	}

	// Increment/Decrement operators /////////////////////////////////////////////

	varint_s<size>& operator++() {				// Prefix
		++m_ustore;
		return *this;
	}

	const varint_s<size> operator++(int) {		// Postfix
		varint_s result(*this);
		++m_ustore;
		return result;
	}

	varint_s<size>& operator--() {				// Prefix
		--m_ustore;
		return *this;
	}

	const varint_s<size> operator--(int) {		// Postfix
		varint_s result(*this);
		--m_ustore;
		return result;
	}


	// Comparison operators ////////////////////////////////////////////////////

	bool operator<(const varint_s<size> rhs) const {
		bool me_neg = m_ustore[size-1];
		bool rhs_neg = rhs.m_ustore[size-1];

		// Am I negative and rhs is positive?
		if (me_neg && !rhs_neg) return true;

		// Am I positive and rhs is negative?
		if (!me_neg && rhs_neg) return false;

		// Otherwise, compare
		return (m_ustore < rhs.m_ustore);
	}

	bool operator<=(const varint_s<size> rhs) const {
		bool me_neg = m_ustore[size-1];
		bool rhs_neg = rhs.m_ustore[size-1];

		// Am I negative and rhs is positive?
		if (me_neg && !rhs_neg) return true;

		// Am I positive and rhs is negative?
		if (!me_neg && rhs_neg) return false;

		// Otherwise, compare
		return (m_ustore <= rhs.m_ustore);
	}

	bool operator>(const varint_s<size> rhs) const {
		bool me_neg = m_ustore[size-1];
		bool rhs_neg = rhs.m_ustore[size-1];

		// Am I negative and rhs is positive?
		if (me_neg && !rhs_neg) return false;

		// Am I positive and rhs is negative?
		if (!me_neg && rhs_neg) return true;

		// Otherwise, compare
		return (m_ustore > rhs.m_ustore);
	}

	bool operator>=(const varint_s<size> rhs) const {
		bool me_neg = m_ustore[size-1];
		bool rhs_neg = rhs.m_ustore[size-1];

		// Am I negative and rhs is positive?
		if (me_neg && !rhs_neg) return false;

		// Am I positive and rhs is negative?
		if (!me_neg && rhs_neg) return true;

		// Otherwise, compare
		return (m_ustore >= rhs.m_ustore);
	}

	bool operator!=(const varint_s<size> rhs) const {
		return m_ustore != rhs.m_ustore;
	}

	bool operator==(const varint_s<size> rhs) const {
		return m_ustore == rhs.m_ustore;
	}

	// Shifting operators //////////////////////////////////////////////////////

	varint_s<size>& operator<<=(int shift_by) {
		m_ustore <<= shift_by;
		return *this;
	}
	varint_s<size> operator<<(int shift_by) const {
		return (varint_s(*this) <<= shift_by);
	}

	varint_s<size>& operator>>=(int shift_by) {
		// What's my sign?
		bool me_neg = m_ustore[size-1];
		
		if (shift_by >= int(size)) {
			*this = me_neg ? varint_s<size>(-1) : varint_s<size>(0);
		}
		else {
			// Shift
			m_ustore >>= shift_by;

			// If I was negative, we need to fill in 1s at the top
			if (me_neg)
				for (uint bit_num=size-shift_by; bit_num<size; bit_num+=64)
					m_ustore.set_bits(bit_num, 64, uint64_t(int64_t(-1)));
		}

		return *this;
	}
	varint_s<size> operator>>(int shift_by) const {
		return (varint_s(*this) >>= shift_by);
	}

	varint_s<size>&	operator<<=(long shift_by)		{ return (*this <<=	int(shift_by));	}
	varint_s<size>	operator<<(long shift_by) const	{ return (*this <<	int(shift_by));	}
	varint_s<size>&	operator>>=(long shift_by)		{ return (*this >>=	int(shift_by));	}
	varint_s<size>	operator>>(long shift_by) const	{ return (*this >>	int(shift_by));	}

	varint_s<size>&	operator<<=(unsigned int shift_by)		{ return (*this <<=	int(shift_by));	}
	varint_s<size>	operator<<(unsigned int shift_by) const	{ return (*this <<	int(shift_by));	}
	varint_s<size>&	operator>>=(unsigned int shift_by)		{ return (*this >>=	int(shift_by));	}
	varint_s<size>	operator>>(unsigned int shift_by) const	{ return (*this >>	int(shift_by));	}

	varint_s<size>&	operator<<=(unsigned long shift_by)			{ return (*this <<=	int(shift_by));	}
	varint_s<size>	operator<<(unsigned long shift_by) const	{ return (*this <<	int(shift_by));	}
	varint_s<size>&	operator>>=(unsigned long shift_by)			{ return (*this >>=	int(shift_by));	}
	varint_s<size>	operator>>(unsigned long shift_by) const	{ return (*this >>	int(shift_by));	}


	// Bitwise operators ///////////////////////////////////////////////////////

	const varint_s<size> operator~() const {
		return varint_s<size>(~m_ustore);
	}

	varint_s<size>& operator&=(const varint_s<size>& rhs) {
		m_ustore &= rhs.m_ustore;
		return *this;
	}
	const varint_s<size> operator&(const varint_s<size>& rhs) const {
		return (varint_s(*this) &= rhs);
	}

	varint_s<size>& operator|=(const varint_s<size>& rhs) {
		m_ustore |= rhs.m_ustore;
		return *this;
	}
	const varint_s<size> operator|(const varint_s<size>& rhs) const {
		return (varint_s(*this) |= rhs);
	}

	varint_s<size>& operator^=(const varint_s<size>& rhs) {
		m_ustore ^= rhs.m_ustore;
		return *this;
	}
	const varint_s<size> operator^(const varint_s<size>& rhs) const {
		return (varint_s(*this) ^= rhs);
	}
	// LogicSource-like cat/slice helpers //////////////////////////////////////

	template <uint size2>
	const varint_u<size+size2> cat(const varint_u<size2>& other) const {
		varint_u<size+size2> rv(m_ustore);
		varint_u<size+size2> other_big(other);

		rv <<= size2;
		rv |= other_big;

		return rv;
	}

	template <uint size2>
	const varint_u<size+size2> cat(const varint_s<size2>& other) const {
		varint_u<size+size2> rv(m_ustore);
		varint_u<size+size2> other_big(other);

		rv <<= size2;
		rv |= other_big;

		return rv;
	}

	template <uint base, uint width>
	const varint_u<width> slice() const {
		return m_ustore.slice<base, width>();
	}

};


} /* end namespace maxcompilersim */

#endif /* VARINT_S_H_ */
