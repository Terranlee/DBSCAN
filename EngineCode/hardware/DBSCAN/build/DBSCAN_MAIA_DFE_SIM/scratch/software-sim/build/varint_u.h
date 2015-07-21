/**
 * @file	varint_u.h
 * @author  Stefan Bach <stefan@maxeler.com>
 *
 * @section DESCRIPTION
 *
 * A templated arbitrary size unsigned integer. This implementation provides
 * overloaded operators and is meant to exhibit identical behavior to unsigned
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

#ifndef VARINT_U_H_
#define VARINT_U_H_

// Helper Macros: these create compiler recognized constant results for
// constant inputs, unlike std::max and std::min
#define MAX(a, b) ( ((a)>(b)) ? (a) : (b) )
#define MIN(a, b) ( ((a)>(b)) ? (b) : (a) )

// Helper Macro: Create a 64 bit mask with the bit at `position' asserted
#define BIT(position) (uint64_t( uint64_t(1) << (position) ))

#include <limits>
#include <iostream>  // iostream must be loaded before boost so that BOOST_NO_STD_LOCALE is defined
#include <boost/format/alt_sstream.hpp>
#include <algorithm>
#include <fstream>
#include <stdexcept>
#include <cstdarg>
#include "HWTypes.h"

namespace maxcompilersim {



template <
	int num_bits, int denom_bits,
	int skip_bits, int frac_bits,
	bool get_remainder, bool hw_exception
>
varint_u<num_bits-skip_bits+frac_bits>
_div_integer(const varint_u<num_bits> &n, const varint_u<denom_bits> &d,
		varint_u<denom_bits+(frac_bits<0?-frac_bits:0)> *remainder, bool *overflow);



CLASS_varint_u {
	friend class varint_s<size>;

	// Chunk size needs to be < 64 and even to allow even splits during multiplication
public:
	enum {
		e_chunk_bits = 62,
		e_num_chunks = size/e_chunk_bits + ((size%e_chunk_bits>0)?1:0)
	};

private:
	static const uint s_chunk_bits = (uint)e_chunk_bits;
	static const uint s_num_chunks = (uint)e_num_chunks;
	static const uint64_t s_mask_low = BIT(s_chunk_bits)-1;
	static const uint64_t s_mask_highest = (size%s_chunk_bits==0) ? s_mask_low : BIT(size%s_chunk_bits)-1;

private:
	uint64_t m_store[s_num_chunks];

public:
	/**
	 * Construct a new unsigned integer of size bits and set it to 0.
	 */
	varint_u() {
		for(uint i=0; i<s_num_chunks; i++)
			m_store[i] = 0;
	}

	/**
	 * Construct a new unsigned integer of size bits and initialize the
	 * lowest of min(64, size) bits with v
	 */
	varint_u(uint64_t v) {
		for(uint i=1; i<s_num_chunks; i++)
			m_store[i] = 0;

		if(size > s_chunk_bits) {
			m_store[0] = v & s_mask_low;
			m_store[1] = v >> s_chunk_bits;
		}
		else
			m_store[0] = v & s_mask_highest;
	}

	/**
	 * Construct a new unsigned integer of size bits and initialize the
	 * internal chunks with the supplied uint64 values. The first argument
	 * must match the expected number of chunks.
	 */
	static varint_u<size> init(size_t n, ...) {
		if (n != s_num_chunks)
			throw std::runtime_error("Expected number of chunks doesn't match indicated number of chunks");

		varint_u<size> new_var;

		va_list args;
		va_start(args, n);
		for (size_t i=0; i<n; ++i)
			new_var.m_store[i] = va_arg(args, uint64_t);
		va_end(args);

		return new_var;
	}

	/**
	 * Construct a new unsigned integer of size bits and initialize
	 * bits offset...min(offset+s_chunk_bits, size) with v. Used internally
	 * for multiplication operations.
	 */
	varint_u(uint64_t v, uint offset) {
		uint length = s_chunk_bits;

		// Set all bits up to the lower half:
		uint l_word_idx = offset/s_chunk_bits;
		uint l_offset = offset%s_chunk_bits;
		uint l_length = s_chunk_bits-l_offset;

		// Check if all bits are out of range to the left
		if (l_word_idx > s_num_chunks)
			l_word_idx = s_num_chunks;

		// Set bits below lower half
		for (uint i=0; i<l_word_idx; i++)
			m_store[i] = 0;

		// Are we done?
		if (l_word_idx == s_num_chunks)
			return;

		// Set lower half
		m_store[l_word_idx] = (v << l_offset) & (BIT(s_chunk_bits)-1);

		if (l_length < length) {								// Did v *not* fit into the lower chunk?
			uint u_word_idx = l_word_idx+1;
			if (u_word_idx < s_num_chunks) {
				m_store[u_word_idx] = v >> l_length;			// Set upper half
				for (uint i=u_word_idx+1; i<s_num_chunks; i++)	// Zero remaining upper bits
					m_store[i] = 0;
			}
		}
		else {									 				// Else: v *did* fit into the lower chunk!
			for (uint i=l_word_idx+1; i<s_num_chunks; i++)		// Zero remaining upper bits
				m_store[i] = 0;
		}
	}

	/**
	 * Construct a new unsigned integer of size bits and initialize the
	 * lowest of min(s_num_chunks, avail_chunks) chunks with v[]. Used internally
	 * for casting operations.
	 */
	varint_u(const uint64_t v[], uint avail_chunks) {
		const uint min_chunks = (s_num_chunks < avail_chunks) ? s_num_chunks : avail_chunks;

		for (uint i=0; i<min_chunks; i++)
			m_store[i] = v[i];

		for (uint i=min_chunks; i<s_num_chunks; i++)
			m_store[i] = 0;
	}

	/**
	 * Construct a new unsigned integer of size bits and initialize the
	 * lowest of min(size, size2) bits with v. Casting between different
	 * sizes is supported through this constructor.
	 */
	template <uint size2>
	varint_u(const varint_u<size2>& v) {
		const uint num_chunks2 = size2/s_chunk_bits + ((size2%s_chunk_bits>0)?1:0);
		const uint min_chunks = (s_num_chunks < num_chunks2) ? s_num_chunks : num_chunks2;

		for (uint i=0; i<min_chunks; i++)
			m_store[i] = v.get_chunk(i);
		for (uint i=min_chunks; i<s_num_chunks; i++)
			m_store[i] = 0;

		if (size<size2)
			m_store[s_num_chunks-1] &= s_mask_highest;
	}

	/**
	 * Construct a new unsigned integer of size bits and initialize the
	 * lowest of min(size, size2) bits with v. Casting between signed/unsigned and
	 * different sizes is supported through this constructor.
	 */
	template <uint size2>
	varint_u(const varint_s<size2>& v) {
		const uint num_chunks2 = size2/s_chunk_bits + ((size2%s_chunk_bits>0)?1:0);
		const uint min_chunks = (s_num_chunks < num_chunks2) ? s_num_chunks : num_chunks2;

		for (uint i=0; i<min_chunks; i++)
			m_store[i] = v.get_chunk(i);
		for (uint i=min_chunks; i<s_num_chunks; i++)
			m_store[i] = 0;

		if (size<size2)
			m_store[s_num_chunks-1] &= s_mask_highest;
	}

	/**
	 * Assign the lowest of min(size, size2) bits with v, set other bits
	 * to 0. Casting between different sizes is supported through this
	 * assignment operator.
	 */
	template <uint size2>
	varint_u<size>& operator=(const varint_u<size2>& v) {
		const uint num_chunks2 = size2/s_chunk_bits + ((size2%s_chunk_bits>0)?1:0);
		const uint min_chunks = (s_num_chunks < num_chunks2) ? s_num_chunks : num_chunks2;

		for (uint i=0; i<min_chunks; i++)
			m_store[i] = v.get_chunk(i);
		for (uint i=min_chunks; i<s_num_chunks; i++)
			m_store[i] = 0;

		if (size<size2)
			m_store[s_num_chunks-1] &= s_mask_highest;

		return *this;
	}


	/**
	 * Assign the lowest of min(size, size2) bits with v, set other bits
	 * to 0. Casting between signed/unsigned and different sizes is
	 * supported through this assignment operator.
	 */
	template <uint size2>
	varint_u<size>& operator=(const varint_s<size2>& v) {
		const uint num_chunks2 = size2/s_chunk_bits + ((size2%s_chunk_bits>0)?1:0);
		const uint min_chunks = (s_num_chunks < num_chunks2) ? s_num_chunks : num_chunks2;

		for (uint i=0; i<min_chunks; i++)
			m_store[i] = v.get_chunk(i);
		for (uint i=min_chunks; i<s_num_chunks; i++)
			m_store[i] = 0;

		if (size<size2)
			m_store[s_num_chunks-1] &= s_mask_highest;

		return *this;
	}

	// Casting and I/O /////////////////////////////////////////////////////////

	operator bool() const {
		bool result = false;
		for (uint i=0; i<s_num_chunks; i++)
			result |= m_store[i];
		return result;
	}

	int64_t toInt64() const {
		int64_t result = get_bits(0, 64);
		return result;
	}
	operator signed char()	const { return static_cast<signed char>(toInt64()); }
	operator short int()	const { return static_cast<short int>(toInt64()); }
	operator int()			const { return static_cast<int>(toInt64()); }
	operator long int()		const { return static_cast<long int>(toInt64()); }

	uint64_t toUInt64() const {
		uint64_t result = get_bits(0, 64);
		return result;
	}
	operator unsigned char()		const { return static_cast<unsigned char>(toUInt64()); }
	operator unsigned short int()	const { return static_cast<unsigned short int>(toUInt64()); }
	operator unsigned int()			const { return static_cast<unsigned int>(toUInt64()); }
	operator unsigned long int()	const { return static_cast<unsigned long int>(toUInt64()); }

	/**
	 * Set the value of an unsigned integer of size bits by reading the
	 * appropriate number of chunks from the supplied ifstream. Returns
	 * true on success, false otherwise.
	 */
	static bool readBinary(varint_u<size> &v, std::ifstream &is) {
		is.read((char*)v.m_store, s_num_chunks * sizeof(uint64_t));
		return (is.gcount() == s_num_chunks * sizeof(uint64_t));
	}

	/**
	 * Set the value of an unsigned integer of size bits by reading the
	 * appropriate number of chunks from the supplied ifstream. Returns
	 * true on success, false otherwise.
	 */
	static bool writeBinary(const varint_u<size> &v, std::ofstream &os) {
		os.write((char*)v.m_store, s_num_chunks * sizeof(uint64_t));
		return os.good();
	}

	friend std::ostream& operator<<(std::ostream& o, const varint_u<size> &v) {
		if(o.flags() & std::ios_base::hex) {
			if(size <= 64) {
				o << v.get_bits(0, 64);
			} else {
				static const varint_u<1> _0(0);
				static const varint_u<4> _f(0xf);

				varint_u<size> num(v);
				boost::io::basic_oaltstringstream<char> oss;

				std::hex(oss);

				if(o.flags() & std::ios_base::uppercase)
					oss.setf(std::ios_base::uppercase);

				if(num == _0)
					oss << '0';

				while(num > _0) {
					oss << (num & _f).get_bits(0, 4);
					num >>= 4;
				}

				std::reverse(oss.begin(), oss.begin() + oss.size());

				if(o.flags() & std::ios_base::showbase)
					o << (o.flags() & std::ios_base::uppercase ? "0X" : "0x");

				o << oss.str();
			}
		} else if(o.flags() & std::ios_base::dec) {
			if(size <= 64) {
				o << v.get_bits(0, 64);
			} else {
				static const varint_u<4> _10(10);
				static const varint_u<1> _0(0);

				varint_u<size> num(v);
				varint_u<4> rem;

				boost::io::basic_oaltstringstream<char> oss;

				if(num == _0)
					oss << '0';

				while(num > _0) {
					num = _div_integer<size,4,0,0,true,false>(num, _10, &rem, NULL);
					oss << (int)rem;
				}

				std::reverse(oss.begin(), oss.begin() + oss.size());

				o << oss.str();
			}
		} else { // std::ios_base::oct -> we mean binary
			if(o.flags() & std::ios_base::showbase)
				o << (o.flags() & std::ios_base::uppercase ? "0B" : "0b");

			for(int i = size-1; i >= 0; i--)
				o << (v.get_bit(i) ? '1' : '0');
		}

		return o;
	}


	// Bit access //////////////////////////////////////////////////////////////

	// Non-checking helper method for copy/assignment operators
	uint64_t get_chunk(uint idx) const {
		return m_store[idx];
	}

	// Non-checking helper method for multiplication
	uint64_t get_half_chunk(uint idx) const {
		uint64_t result = m_store[idx/2];
		if (idx%2 > 0)							// Upper half
			result >>= (s_chunk_bits/2);
		else									// Lower half
			result &= (BIT(s_chunk_bits/2)-1);
		return result;
	}

	uint64_t get_bits(int base, int length) const {
		// Sanity check
		if (length > 64)
			length = 64;
		// if (length < 1) <-- We check this later anyway!

		// We return '0's for out-of-range bits, check right end:
		uint zeros_right = 0;
		if (base < 0) {
			zeros_right = -base;
			length -= zeros_right;
			base = 0;
		}

		// Maybe all bits are far out right
		if (length<1)
			return 0;

		// Check left end, maybe all bits are out of range
		if (base >= (int)size)
			return 0;
		// If only some are out of range we just cut down the length
		if (base+length > (int)size)
			length = size-base;

		// Gather the result
		uint64_t result = 0;

		// Lower half
		uint l_word_idx = base/s_chunk_bits;
		uint l_offset = base%s_chunk_bits;
		uint l_length = s_chunk_bits-l_offset;

		result = m_store[l_word_idx] >> l_offset;

		// Upper half
		uint u_word_idx = l_word_idx+1;
		if (u_word_idx < s_num_chunks && (int)l_length < length) {
			uint u_padding = l_length;

			result |= m_store[u_word_idx] << u_padding;
		}

		// Truncate additional upper bits (length has already been
		// lowered by zeros_right if appropriate!
		if (length < 64)
			result &= BIT(length)-1;

		return result << zeros_right;
	}

	template<uint base, uint length, uint size2>
	void set_bits(varint_s<size2> value)
	{ set_bits<base, length, size2> (varint_u<size2>(value)); }

	template<uint base, uint length, uint size2>
	void set_bits(varint_u<size2> value) {
		const int iterations = length / 64;
		const int remainder = length % 64;
		for (int index = 0; index < iterations; index++) {
			set_bits(base + (index * 64), 64, value.get_bits((index*64),64));
		}
		if (remainder != 0)
			set_bits(base + iterations * 64, remainder,
				value.get_bits(iterations * 64, remainder));
	}

	void set_bits(int base, int length, uint64_t value) {
		// Sanity check
		if (length > 64)
			length = 64;
		// if (length < 1) <-- We check this later anyway!

		// We cannot set out-of-range bits, check right end:
		if (base < 0) {
			int discard_right = -base;
			length -= discard_right;
			value >>= discard_right;
			base = 0;
		}

		// Maybe all bits are far out right
		if (length<1)
			return;

		// Check left end, maybe all bits are out of range
		if (base >= (int)size)
			return;
		// If only some are out of range we just cut down the length
		if (base+length > (int)size)
			length = size-base;

		// Apply the new bits, lower half:
		uint l_word_idx = base/s_chunk_bits;
		uint l_offset = base%s_chunk_bits;
		uint l_length = s_chunk_bits-l_offset;
		if (l_length > (uint)length)
			l_length = length;

		// Mask of bits to be set
		uint64_t l_mask = (BIT(l_offset+l_length)-1) ^ (BIT(l_offset)-1);

		m_store[l_word_idx] &= ~l_mask;							// Clear out current bits
		m_store[l_word_idx] |= (value << l_offset) & l_mask;	// Apply new values

		// Upper half
		uint u_word_idx = l_word_idx+1;
		if (u_word_idx < s_num_chunks && (int)l_length < length) {
			uint u_length = length - l_length;

			uint64_t u_mask = BIT(u_length)-1;
			m_store[u_word_idx] &= ~u_mask;
			m_store[u_word_idx] |= (value >> l_length) & u_mask;
		}
	}

	bool get_bit(int base) const {
		if (base < 0 || base >= (int)size)
			return false;

		uint word_idx = base/s_chunk_bits;
		uint offset = base%s_chunk_bits;

		uint64_t result = (m_store[word_idx] >> offset) & 0x1l;
		return result;
	}

	void set_bit(int base, bool value) {
		if (base < 0 || base >= (int)size)
			return;

		uint word_idx = base/s_chunk_bits;
		uint offset = base%s_chunk_bits;

		uint64_t value64 = uint64_t(value) << offset;		// Value to be set
		uint64_t mask = BIT(offset);						// Mask of bit to be set

		m_store[word_idx] &= ~mask;							// Clear out current bit
		m_store[word_idx] |= value64;						// Apply new values
	}

	class bit_inspector {
	private:
		friend class varint_u<size>;
		const varint_u<size>& m_varint;
		uint m_bit;
		bit_inspector(const varint_u<size>& varint, uint bit)
			: m_varint(varint), m_bit(bit) {}
	public:
		operator bool() const { return m_varint.get_bit(m_bit); }
	};
	const bit_inspector operator[] (unsigned idx) const {
		return bit_inspector(*this, idx);
	}

	class bit_setter {
	private:
		friend class varint_u<size>;
		varint_u<size>& m_varint;
		uint m_bit;
		bit_setter(varint_u<size>& varint, uint bit)
			: m_varint(varint), m_bit(bit) {}
	public:
		operator bool() const { return m_varint.get_bit(m_bit); }
		bool operator= (bool value) { m_varint.set_bit(m_bit, value); return value; }
	};
	bit_setter operator[] (unsigned idx) {
		return bit_setter(*this, idx);
	}

	// Arithmetic operators ////////////////////////////////////////////////////

	const varint_u<size> operator+() const {
		return *this;
	}

	const varint_u<size> operator-() const {
		varint_u ones_compl = ~(*this);
		return ++ones_compl;
	}

	varint_u<size>& operator+=(const varint_u<size>& rhs) {
		uint64_t carry = 0;

		// Lower chunks
		for(uint i=0; i<s_num_chunks-1; i++) {
			m_store[i] += rhs.m_store[i] + carry;
			carry = m_store[i] >> s_chunk_bits;
			m_store[i] &= s_mask_low;
		}

		// Highest chunk
		m_store[s_num_chunks-1] += rhs.m_store[s_num_chunks-1] + carry;
		m_store[s_num_chunks-1] &= s_mask_highest;

		return *this;
	}
	const varint_u<size> operator+(const varint_u<size>& rhs) const {
		return (varint_u(*this) += rhs);
	}

	varint_u<size>& operator-=(const varint_u<size>& rhs) {
		return *this += (-rhs);
	}
	const varint_u<size> operator-(const varint_u<size>& rhs) const {
		return (varint_u(*this) -= rhs);
	}

	const varint_u<size> operator*(const varint_u<size>& rhs) const {
		/*
		 * To multiply to numbers j, k with half-chunks
		 *   j = EDCBA, k = ZYXWV
		 *  we enumerate all possible pairs of half-chunks as follows:
		 *   EZ  EY  EX  EW  EV
		 *   DZ  DY  DX  DW  DV
		 *   CZ  CY  CX  CW  CV
		 *   BZ  BY  BX  BW  BV
		 *   AZ  AY  AX  AW  AV
		 *  Only the lower right triangle (incl. the diagonal) are of
		 *  interest, as all combinations above the diagonal are out of
		 *  range anyway.
		 *
		 *  We calculate all partial multiplications independently and
		 *  accumulate their results.
		 */
		uint half_chunk_bits = s_chunk_bits / 2;
		uint num_half_chunks = size/half_chunk_bits  + ((size%half_chunk_bits >0)?1:0);

		varint_u<size> result;

		for (uint i=0; i<num_half_chunks; i++)
			for (uint j=0; j<num_half_chunks-i; j++) {
				uint64_t partial = get_half_chunk(i) * rhs.get_half_chunk(j);
				result += varint_u<size>(partial, (i+j)*half_chunk_bits);
			}

		return result;
	}
	varint_u<size>& operator*=(const varint_u<size>& rhs) {
		varint_u<size> tmp = (*this * rhs);
		(*this) = tmp;

		return *this;
	}

//	const varint_u<size> operator/(const varint_u<size>& rhs) const {
//		COMPILER_CHECK(false, VarintUnsigned_Division_not_implemented);
//	}
//	varint_u<size>& operator/=(const varint_u<size>& rhs) {
//		COMPILER_CHECK(false, VarintUnsigned_Division_not_implemented);
//	}
//	const varint_u<size> operator%(const varint_u<size>& rhs) const {
//		COMPILER_CHECK(false, VarintUnsigned_Modulo_not_implemented);
//	}
//	varint_u<size>& operator%=(const varint_u<size>& rhs) {
//		COMPILER_CHECK(false, VarintUnsigned_Modulo_not_implemented);
//	}

	// Increment/Decrement operators /////////////////////////////////////////////

	varint_u<size>& operator++() {				// Prefix
		uint64_t carry = 1;

		// Lower chunks
		for(uint i=0; i<s_num_chunks-1; i++) {
			m_store[i] += carry;
			carry = m_store[i] >> s_chunk_bits;
			m_store[i] &= s_mask_low;
		}

		// Highest chunk
		m_store[s_num_chunks-1] += carry;
		m_store[s_num_chunks-1] &= s_mask_highest;

		return *this;
	}

	const varint_u<size> operator++(int) {		// Postfix
		varint_u result(*this);
		++(*this);
		return result;
	}

	varint_u<size>& operator--() {				// Prefix
		*this += ~varint_u();
		return *this;
	}

	const varint_u<size> operator--(int) {		// Postfix
		varint_u result(*this);
		--(*this);
		return result;
	}


	// Comparison operators ////////////////////////////////////////////////////

	bool operator<(const varint_u<size> rhs) const {
		for(int i=s_num_chunks-1; i>-1; i--) {
			if (m_store[i] < rhs.m_store[i])		// Less than
				return true;
			if (m_store[i] > rhs.m_store[i])		// Greater then
				return false;
		}
		return false;								// Equal
	}

	bool operator<=(const varint_u<size> rhs) const {
		for(int i=s_num_chunks-1; i>-1; i--) {
			if (m_store[i] < rhs.m_store[i])
				return true;						// Less than
			else if (m_store[i] > rhs.m_store[i])
				return false;						// Greater than
		}
		return true;								// Equal
	}

	bool operator>(const varint_u<size> rhs) const {
		for(int i=s_num_chunks-1; i>-1; i--) {
			if (m_store[i] > rhs.m_store[i])		// Greater than
				return true;
			if (m_store[i] < rhs.m_store[i])		// Less than
				return false;
		}
		return false;								// Equal
	}

	bool operator>=(const varint_u<size> rhs) const {
		for(int i=s_num_chunks-1; i>-1; i--) {
			if (m_store[i] > rhs.m_store[i])		// Greater than
				return true;
			else if (m_store[i] < rhs.m_store[i])	// Less than
				return false;
		}
		return true;								// Equal
	}

	bool operator!=(const varint_u<size> rhs) const {
		for (uint i=0; i<s_num_chunks; i++)
			if (m_store[i] != rhs.m_store[i])		// Not Equal
				return true;
		return false;								// Equal
	}

	bool operator==(const varint_u<size> rhs) const {
		for (uint i=0; i<s_num_chunks; i++)
			if (m_store[i] != rhs.m_store[i])		// Not Equal
				return false;
		return true;								// Equal
	}

	// Shifting operators //////////////////////////////////////////////////////

	varint_u<size>& operator<<=(int shift_by) {
		for(int i=s_num_chunks-1; i>-1; i--)
			m_store[i] = get_bits((i*s_chunk_bits)-shift_by, s_chunk_bits);
		m_store[s_num_chunks-1] &= s_mask_highest;
		return *this;
	}
	varint_u<size> operator<<(int shift_by) const {
		return (varint_u(*this) <<= shift_by);
	}

	varint_u<size>& operator>>=(int shift_by) {
		for(uint i=0; i<s_num_chunks; i++)
			m_store[i] = get_bits((i*s_chunk_bits)+shift_by, s_chunk_bits);
		return *this;
	}
	varint_u<size> operator>>(int shift_by) const {
		return (varint_u(*this) >>= shift_by);
	}

	varint_u<size>&	operator<<=(long shift_by)		{ return (*this <<=	int(shift_by));	}
	varint_u<size>	operator<<(long shift_by) const	{ return (*this <<	int(shift_by));	}
	varint_u<size>&	operator>>=(long shift_by)		{ return (*this >>=	int(shift_by));	}
	varint_u<size>	operator>>(long shift_by) const	{ return (*this >>	int(shift_by));	}

	varint_u<size>&	operator<<=(unsigned int shift_by)		{ return (*this <<=	int(shift_by));	}
	varint_u<size>	operator<<(unsigned int shift_by) const	{ return (*this <<	int(shift_by));	}
	varint_u<size>&	operator>>=(unsigned int shift_by)		{ return (*this >>=	int(shift_by));	}
	varint_u<size>	operator>>(unsigned int shift_by) const	{ return (*this >>	int(shift_by));	}

	varint_u<size>&	operator<<=(unsigned long shift_by)			{ return (*this <<=	int(shift_by));	}
	varint_u<size>	operator<<(unsigned long shift_by) const	{ return (*this <<	int(shift_by));	}
	varint_u<size>&	operator>>=(unsigned long shift_by)			{ return (*this >>=	int(shift_by));	}
	varint_u<size>	operator>>(unsigned long shift_by) const	{ return (*this >>	int(shift_by));	}


	// Bitwise operators ///////////////////////////////////////////////////////

	const varint_u<size> operator~() const {
		varint_u<size> r;
		for(uint i=0; i<s_num_chunks-1; i++)
			r.m_store[i] = ~(m_store[i]) & s_mask_low;
		r.m_store[s_num_chunks-1] = ~(m_store[s_num_chunks-1]) & s_mask_highest;
		return r;
	}

	varint_u<size>& operator&=(const varint_u<size>& rhs) {
		for(uint i=0; i<s_num_chunks; i++)
			m_store[i] &= rhs.m_store[i];
		return *this;
	}
	const varint_u<size> operator&(const varint_u<size>& rhs) const {
		return (varint_u(*this) &= rhs);
	}

	varint_u<size>& operator|=(const varint_u<size>& rhs) {
		for(uint i=0; i<s_num_chunks; i++)
			m_store[i] |= rhs.m_store[i];
		return *this;
	}
	const varint_u<size> operator|(const varint_u<size>& rhs) const {
		return (varint_u(*this) |= rhs);
	}

	varint_u<size>& operator^=(const varint_u<size>& rhs) {
		for(uint i=0; i<s_num_chunks; i++)
			m_store[i] ^= rhs.m_store[i];
		return *this;
	}
	const varint_u<size> operator^(const varint_u<size>& rhs) const {
		return (varint_u(*this) ^= rhs);
	}
	// LogicSource-like cat/slice helpers //////////////////////////////////////

	template <uint size2>
	const varint_u<size+size2> cat(const varint_u<size2>& other) const {
		varint_u<size+size2> rv(*this);
		varint_u<size+size2> other_big(other);

		rv <<= size2;
		rv |= other_big;

		return rv;
	}

	template <uint size2>
	const varint_u<size+size2> cat(const varint_s<size2>& other) const {
		varint_u<size+size2> rv(*this);
		varint_u<size+size2> other_big(other);

		rv <<= size2;
		rv |= other_big;

		return rv;
	}

	template <uint base, uint width>
	const varint_u<width> slice() const {
		varint_u<size> shifted(*this);
		shifted >>= base;

		varint_u<width> rv(shifted);
		return rv;
	}
};

template <
	int num_bits, int denom_bits,
	int skip_bits, int frac_bits,
	bool get_remainder, bool hw_exception
>
varint_u<num_bits-skip_bits+frac_bits>
_div_integer(const varint_u<num_bits> &n, const varint_u<denom_bits> &d,
		varint_u<denom_bits+(frac_bits<0?-frac_bits:0)> *remainder, bool *overflow)
{
	// See: MaxDCIntegerDiv.java

	// Initialize intermediates ////////////////////////////////////////////

	// For simplicity's sake we operate on two (1+denom_bits+num_bits) wide numbers,
	// which will make the skipping and shifting operation much easier

	varint_s<1+denom_bits+num_bits> lrg_PN(n);
	lrg_PN <<= skip_bits;

	varint_s<1+denom_bits+num_bits> lrg_D(d);
	lrg_D <<= num_bits;

	// Check for overflow if required //////////////////////////////////////

	// We made live very easy for ourselves by gluing P and N together,
	// but now we have to be careful because
	// * with enough skipped bits the initial P might appear to be negative
	// * with even more skip, we might have shifted bits out of the number

	if (hw_exception) {
		*overflow = (lrg_PN >= lrg_D);
		*overflow |= lrg_PN[denom_bits+num_bits];

		if (skip_bits - denom_bits > 0) {
			/*
			 * With optimizations turned off the compiler becomes too dumb
			 * to notice that when skip_bits-denom_bits<=0 this code will never be executed
			 * and will try to allocate a negative amount of space on the stack for
			 * varint_u's of negative size. Segmentation fault.
			 * Hence, front_msb_checked is needed.
			 */
			const int front_msb = MIN(skip_bits - denom_bits, num_bits);
			const uint front_msb_checked = front_msb < 0 ? 0 : front_msb;

			varint_u<front_msb_checked> front = n.slice<num_bits-front_msb_checked,front_msb_checked>();
			*overflow |= ((bool)front);
		}
	}

	// Loop to produce the quotient bits ///////////////////////////////////

	const int quot_bits = num_bits - skip_bits + frac_bits;
	varint_u<quot_bits> q;

	bool last_p_positive = true;
	for (int i=0; i<quot_bits; i++) {
		// Left shift P and shift the next bit of N into P on the right (easy now!)
		lrg_PN <<= 1;

		// Add or sub the denominator depending on the (shifted out) sign of P
		lrg_PN = last_p_positive ? (lrg_PN - lrg_D) : (lrg_PN + lrg_D);

		// Append the negated new sign to the partial quotient
		last_p_positive = ! ((bool)lrg_PN[denom_bits+num_bits]);

		q <<= 1;
		if (last_p_positive)
			q++;
	}

	// Output the final quotient and the remainder /////////////////////////

	// If the remainder should be supplied, it can be found in P. But
	// it needs to be corrected (restored) in case it's negative.
	//
	// If a negative amount of fractional bits was requested, the last
	// couple of numerator bits have to be glued to the restored remainder

	if (get_remainder) {
		// Restore the remainder if necessary
		if (! last_p_positive)
			lrg_PN += lrg_D;

		// Slice out the actual P (cut off sign bit and numerator)
		const int rem_bits = denom_bits + (frac_bits<0 ? -frac_bits : 0);
		const int rem_base = num_bits + (frac_bits<0 ? frac_bits : 0);

		*remainder = lrg_PN.slice<rem_base,rem_bits>();
	}

	return q;
}



template <
	uint r_size, int r_offset,
	RoundMode round_mode,
	uint a_size, int a_offset,
	uint b_size, int b_offset,
	bool hw_exception
>
varint_u<r_size>
_div_unsigned(
		const varint_u<a_size> &a,
		const varint_u<b_size> &b,
		bool *overflow,
		bool *round_bit,
		bool *sticky_bit) {
	// See: MaxDCUnsignedDiv.java,
	// overflow detection has been moved to the integer layer

	// Work out the DividerConfig //////////////////////////////////////////

	// Numerator and Denominator are fixed according to input types,
	const int num_bits = a_size;
	const int denom_bits = b_size;

	// Work out idiv's quotient type with right-hand side adjusted,
	// also consider the rounding mode's additional fraction bit
	const int frac_bits = (a_offset - b_offset) - r_offset + (round_mode != TRUNCATE ? 1 : 0);
	const int tmp_quot_bits = a_size + ((a_offset - b_offset) - r_offset) + (round_mode != TRUNCATE ? 1 : 0);
	const int quot_off = r_offset - (round_mode != TRUNCATE ? 1 : 0);

	// Adjust the left hand end of the quotient type
	const int cutoff_msb = (quot_off + tmp_quot_bits) - (r_offset + r_size);
	const int skip_bits = (cutoff_msb > 0) ? cutoff_msb : 0;
	const int quot_bits = tmp_quot_bits - skip_bits;

	// Instantiate the integer divider /////////////////////////////////////

	varint_u<r_size> quot;

	if (quot_bits > 0) {
		/*
		 * With optimizations turned off the compiler becomes too dumb
		 * to notice that when quot_bits<=0 this code will never be executed
		 * and will try to allocate a negative amount of space on the stack for
		 * varint_u's of negative size. Segmentation fault.
		 */
		const int quot_bits_checked = quot_bits>0 ? quot_bits : 1;
		const int skip_bits_checked = quot_bits>0 ? skip_bits : 0;
		const int frac_bits_checked = quot_bits>0 ? frac_bits : 0;

		const int rem_bits = denom_bits+(frac_bits_checked<0?-frac_bits_checked:0);
		varint_u<rem_bits> remainder;

		varint_u<quot_bits_checked> idiv_quot =	_div_integer<num_bits, denom_bits,
				skip_bits_checked, frac_bits_checked,
				true, hw_exception>(a, b, &remainder, overflow);

		// Perform rounding as required ////////////////////////////////////////
		*sticky_bit = (bool)remainder;

		if (round_mode != TRUNCATE) {
			*round_bit = idiv_quot[0];
			idiv_quot >>= 1;

			if (round_mode == TONEAREVEN) {
				bool lsb = idiv_quot[0];
				*round_bit = *round_bit && (*sticky_bit || lsb);
			}

			varint_u<r_size> unrounded = idiv_quot;

			// Add the final round-bit to the quotient, check for overflow
			// in case of hardware exceptions
			quot = unrounded;
			if (*round_bit)
				quot++;

			if (hw_exception && (unrounded > quot))
					*overflow = true;
		}
		else {
			quot = idiv_quot;
		}
	}
	else {
		// Manually set sticky bit for rounding fix
		// if we didn't do a division
		if (round_mode == TRUNCATE) {
			*sticky_bit = (bool) a;
		}
	}

	return quot;
}


} /* end namespace maxcompilersim */

#undef MAX /* end Helper Macro */
#undef MIN
#undef BIT

#endif /* VARINT_U_H_ */
