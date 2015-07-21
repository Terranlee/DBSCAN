#ifndef DYNAMIC_VARINT_U_H_
#define DYNAMIC_VARINT_U_H_

#include <fstream>
#include "HWTypes.h"
#include "SimException.h"
#include <map>
#include <stack>
// Helper Macro: Create a 64 bit mask with the bit at `position' asserted
#define BIT(position) (uint64_t( uint64_t(1) << position ))

namespace maxcompilersim {

// Most of this is based on varint_u, if anything looks strange refer to that.
class DynamicVarUInt {
private:
	typedef uint64_t t_store;

	static const uint     s_chunk_bits = 62;
	static const uint64_t s_mask_low = BIT(s_chunk_bits)-1;

	static class t_pool_wrap
	{
		typedef std::stack<t_store *> t_pool_stack;
		typedef std::map<uint, t_pool_stack> t_pool_map;
		t_pool_map m_pool_map;

	public:
		t_store *alloc(uint size);
		void free(uint size, t_store *element_to_free);
		~t_pool_wrap();
	} s_pool;

	uint      m_num_chunks;
	uint64_t  m_mask_highest;
	uint      m_size;
	t_store  *m_store;

	void assertNumeric(int line, const char *func) const;

public:
	~DynamicVarUInt();

	// Initialises a not-a-number instance which can be used with containers/arrays.
	// No actual operations with instances initialised in this way will work except
	// assignment and copy-constructors.
	DynamicVarUInt();

	explicit DynamicVarUInt(uint bit_size);
	DynamicVarUInt(uint bit_size, uint64_t v);

	/**
	 * Construct a new unsigned integer of size bits and initialize
	 * bits offset...min(offset+s_chunk_bits, size) with v. Used internally
	 * for multiplication operations.
	 */
	DynamicVarUInt(uint bit_size, uint64_t v, uint offset);
	DynamicVarUInt(const DynamicVarUInt& v);


	template <uint vsize>
	DynamicVarUInt(const varint_u<vsize>& v) :
		m_num_chunks  ( vsize/s_chunk_bits + ((vsize%s_chunk_bits>0)?1:0) ),
		m_mask_highest( (vsize%s_chunk_bits==0) ? s_mask_low : BIT(vsize%s_chunk_bits)-1 ),
		m_size(vsize),
		m_store(m_size ? s_pool.alloc(m_num_chunks) : 0)
	{
		for(uint i = 0; i < m_num_chunks; i++)
			m_store[i] = v.get_chunk(i);
	}

	DynamicVarUInt(uint bit_size, const DynamicVarUInt& v);
	uint size() const;

	template <uint vsize>
	varint_u<vsize> toVarIntU() const {
		return varint_u<vsize>(m_store, m_num_chunks);
	}

	DynamicVarUInt& operator=(const DynamicVarUInt& v);


	template<uint vsize>
	DynamicVarUInt& operator=(const varint_u<vsize>& v) {
		if(!m_size) {
			m_size = vsize;
			m_num_chunks = m_size/s_chunk_bits + ((m_size%s_chunk_bits>0)?1:0);
			m_mask_highest = (m_size%s_chunk_bits==0) ? s_mask_low : BIT(m_size%s_chunk_bits)-1;

			m_store = m_size ? s_pool.alloc(m_num_chunks) : 0;
		} else if (m_size != vsize)
			BOOST_THROW_EXCEPTION(
				SimException("When assigning to a DynamicVarUInt "
								"LHS.size != RHS.size"));

		for(uint i = 0; i < m_num_chunks; i++)
			m_store[i] = v.get_chunk(i);

		return *this;
	}

	DynamicVarUInt& operator=(const uint64_t v);

	// Casting and I/O /////////////////////////////////////////////////////////

	operator bool() const;

	int64_t toInt64() const;
	operator signed char() const;
	operator short int() const;
	operator int() const;
	operator long int() const;

	uint64_t toUInt64() const;
	operator unsigned char()		const;
	operator unsigned short int()	const;
	operator unsigned int()			const;
	operator unsigned long int()	const;

	/**
	 * Set the value of an unsigned integer of size bits by reading the
	 * appropriate number of chunks from the supplied ifstream. Returns
	 * true on success, false otherwise.
	 */
	bool readBinary(std::ifstream &is);

	/**
	 * Set the value of an unsigned integer of size bits by reading the
	 * appropriate number of chunks from the supplied ifstream. Returns
	 * true on success, false otherwise.
	 */
	bool writeBinary(std::ofstream &os) const;

	/**
	 * Read from file in plain binary format. Data is assumed to be
	 * zero-extended to the next multiple of 8 bits.
	 */
	bool readPlainBinary(std::ifstream &is);

	/**
	 * Write to file in plain binary format. Data is zero-extended to the
	 * next multiple of 8 bits.
	 */
	bool writePlainBinary(std::ofstream &os) const;

	friend std::ostream& operator<<(std::ostream& o, const DynamicVarUInt &v);


	// Bit access //////////////////////////////////////////////////////////////

	// Non-checking helper method for copy/assignment operators
	t_store get_chunk(uint idx) const;

	// Non-checking helper method for multiplication
	t_store get_half_chunk(uint idx) const;

	t_store get_bits(int base, int length) const;
	void set_bits(int base, int length, uint64_t value);
	bool get_bit(int base) const;
	void set_bit(int base, bool value);

	class bit_inspector {
	private:
		friend class DynamicVarUInt;
		const DynamicVarUInt& m_varint;
		uint m_bit;
		bit_inspector(const DynamicVarUInt& varint, uint bit);
	public:
		operator bool() const;
	};

	const bit_inspector operator[] (unsigned idx) const;

	class bit_setter {
	private:
		friend class DynamicVarUInt;
		DynamicVarUInt& m_varint;
		uint m_bit;
		bit_setter(DynamicVarUInt& varint, uint bit);
	public:
		operator bool() const;
		bool operator= (bool value);
	};
	bit_setter operator[] (unsigned idx);

	// Arithmetic operators ////////////////////////////////////////////////////

	const DynamicVarUInt operator+() const;
	const DynamicVarUInt operator-() const;
	DynamicVarUInt& operator+=(const DynamicVarUInt& rhs);
	const DynamicVarUInt operator+(const DynamicVarUInt& rhs) const;
	DynamicVarUInt& operator-=(const DynamicVarUInt& rhs);
	const DynamicVarUInt operator-(const DynamicVarUInt& rhs) const;
	const DynamicVarUInt operator*(const DynamicVarUInt& rhs) const;
	DynamicVarUInt& operator*=(const DynamicVarUInt& rhs);

	// Increment/Decrement operators /////////////////////////////////////////////

	DynamicVarUInt& operator++();
	const DynamicVarUInt operator++(int);
	DynamicVarUInt& operator--();
	const DynamicVarUInt operator--(int);


	// Comparison operators ////////////////////////////////////////////////////

	bool operator<(const DynamicVarUInt rhs) const;
	bool operator<=(const DynamicVarUInt rhs) const;
	bool operator>(const DynamicVarUInt rhs) const;
	bool operator>=(const DynamicVarUInt rhs) const;
	bool operator!=(const DynamicVarUInt rhs) const;
	bool operator==(const DynamicVarUInt rhs) const;

	// Shifting operators //////////////////////////////////////////////////////

	DynamicVarUInt& operator<<=(int shift_by);
	DynamicVarUInt operator<<(int shift_by) const;
	DynamicVarUInt& operator>>=(int shift_by);
	DynamicVarUInt operator>>(int shift_by) const;
	DynamicVarUInt&	operator<<=(long shift_by);
	DynamicVarUInt	operator<<(long shift_by) const;
	DynamicVarUInt&	operator>>=(long shift_by);
	DynamicVarUInt	operator>>(long shift_by) const;

	DynamicVarUInt&	operator<<=(unsigned int shift_by);
	DynamicVarUInt	operator<<(unsigned int shift_by) const;
	DynamicVarUInt&	operator>>=(unsigned int shift_by);
	DynamicVarUInt	operator>>(unsigned int shift_by) const;

	DynamicVarUInt&	operator<<=(unsigned long shift_by);
	DynamicVarUInt	operator<<(unsigned long shift_by) const;
	DynamicVarUInt&	operator>>=(unsigned long shift_by);
	DynamicVarUInt	operator>>(unsigned long shift_by) const;


	// Bitwise operators ///////////////////////////////////////////////////////

	const DynamicVarUInt operator~() const;
	DynamicVarUInt& operator&=(const DynamicVarUInt& rhs);
	const DynamicVarUInt operator&(const DynamicVarUInt& rhs) const;
	DynamicVarUInt& operator|=(const DynamicVarUInt& rhs);
	const DynamicVarUInt operator|(const DynamicVarUInt& rhs) const;
	DynamicVarUInt& operator^=(const DynamicVarUInt& rhs);
	const DynamicVarUInt operator^(const DynamicVarUInt& rhs) const;

	// LogicSource-like cat/slice helpers //////////////////////////////////////

	const DynamicVarUInt cat(const DynamicVarUInt& other) const;
	const DynamicVarUInt slice(uint base, uint width) const;
};


} /* end namespace maxcompilersim */

#undef BIT /* end Helper Macro */


#endif /* DYNAMIC_VARINT_U_H_ */
