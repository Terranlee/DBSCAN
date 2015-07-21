
#ifndef STREAMSTATUS_H_
#define STREAMSTATUS_H_

#include "ManagerSync.h"
#include "PushSync.h"
#include "PullSync.h"
#include "Data.h"					// defines Data
#include "MappedElements.h"			// defines MappedRegisterPtr

#include <boost/smart_ptr/shared_ptr.hpp>


namespace maxcompilersim {

/* following regs are defined:
	REG(STREAMSTATUS_host_to_dram_host_to_dram.StreamStatus_OUT_working_cycles, 0x85, 6, hwBits(48))
	REG(STREAMSTATUS_host_to_dram_host_to_dram.StreamStatus_OUT_waiting_cycles, 0x8b, 6, hwBits(48))
	REG(STREAMSTATUS_host_to_dram_host_to_dram.StreamStatus_OUT_active_cycles, 0x91, 6, hwBits(48))
	REG(STREAMSTATUS_host_to_dram_host_to_dram.StreamStatus_OUT_flags, 0x97, 1, hwBits(8))
*/

#define STRING_STREAMSTATUS_WORKING_CYCLES "StreamStatus_OUT_working_cycles"
#define STRING_STREAMSTATUS_WAITING_CYCLES "StreamStatus_OUT_waiting_cycles"
#define STRING_STREAMSTATUS_ACTIVE_CYCLES "StreamStatus_OUT_active_cycles"
#define STRING_STREAMSTATUS_FLAGS "StreamStatus_OUT_flags"
#define STRING_STREAMSTATUS_CHECKSUM "StreamStatus_OUT_checksum"

/* the flags map to following bitfield
#ifdef REG_BITFIELD
#define REG_BITFIELD_PRESENT 1
	REG_BITFIELD(STREAMSTATUS_host_to_dram_host_to_dram.StreamStatus_OUT_flags, error_working_and_waiting, 0, 1)
	REG_BITFIELD(STREAMSTATUS_host_to_dram_host_to_dram.StreamStatus_OUT_flags, error_working_after_done, 1, 1)
	REG_BITFIELD(STREAMSTATUS_host_to_dram_host_to_dram.StreamStatus_OUT_flags, error_fifo_overflow, 2, 1)
	REG_BITFIELD(STREAMSTATUS_host_to_dram_host_to_dram.StreamStatus_OUT_flags, error_fifo_underflow, 3, 1)
	REG_BITFIELD(STREAMSTATUS_host_to_dram_host_to_dram.StreamStatus_OUT_flags, is_push_interface, 4, 1)
	REG_BITFIELD(STREAMSTATUS_host_to_dram_host_to_dram.StreamStatus_OUT_flags, is_waiting, 5, 1)
	REG_BITFIELD(STREAMSTATUS_host_to_dram_host_to_dram.StreamStatus_OUT_flags, is_done, 6, 1)
	REG_BITFIELD(STREAMSTATUS_host_to_dram_host_to_dram.StreamStatus_OUT_flags, fifo_exists, 7, 1)
#endif
*/

#define STRING_STREAMSTATUS_FLAGS_ERROR_WORKING_AND_WAITING "error_working_and_waiting"
#define POSITION_STREAMSTATUS_FLAGS_ERROR_WORKING_AND_WAITING 0
#define STRING_STREAMSTATUS_FLAGS_ERROR_WORKING_AFTER_DONE "error_working_after_done"
#define POSITION_STREAMSTATUS_FLAGS_ERROR_WORKING_AFTER_DONE 1
#define STRING_STREAMSTATUS_FLAGS_ERROR_FIFO_OVERFLOW "error_fifo_overflow"
#define POSITION_STREAMSTATUS_FLAGS_ERROR_FIFO_OVERFLOW 2
#define STRING_STREAMSTATUS_FLAGS_ERROR_FIFO_UNDERFLOW "error_fifo_underflow"
#define POSITION_STREAMSTATUS_FLAGS_ERROR_FIFO_UNDERFLOW 3

#define STRING_STREAMSTATUS_FLAGS_IS_PUSH_INTERFACE "is_push_interface"
#define POSITION_STREAMSTATUS_FLAGS_IS_PUSH_INTERFACE 4
#define STRING_STREAMSTATUS_FLAGS_IS_WAITING "is_waiting"
#define POSITION_STREAMSTATUS_FLAGS_IS_WAITING 5
#define STRING_STREAMSTATUS_FLAGS_IS_DONE "is_done"
#define POSITION_STREAMSTATUS_FLAGS_IS_DONE 6
#define STRING_STREAMSTATUS_FLAGS_FIFO_EXISTS "fifo_exists"
#define POSITION_STREAMSTATUS_FLAGS_FIFO_EXISTS 7

#define STREAMSTATUS_WIDTH	(32)
#define STREAMSTATUS_MASK	((1L << STREAMSTATUS_WIDTH) - 1)


/*
 * Base class for StreamStatusSync component.
 *
 */

class StreamStatusSync :
	public virtual ManagerBlockSync
{
private:
	MappedRegisterPtr m_working_cycles;
	MappedRegisterPtr m_waiting_cycles;
	MappedRegisterPtr m_active_cycles;
	MappedRegisterPtr m_flags;
	std::vector<MappedRegisterPtr> m_checksum_registers;
	uint64_t          m_number_cycles;
	uint64_t          m_number_waiting_cycles;
	const uint32_t    m_wait_latency;
	uint32_t          m_wait_latency_counter;
	bool              m_done_asserted;
	bool              m_error_working_and_waiting;
	bool              m_error_working_after_done;

	// double buffering of register values
	uint64_t          m_active_cycles_buffer;
	uint64_t          m_waiting_cycles_buffer;
	uint64_t          m_working_cycles_buffer;
	std::vector<uint64_t> m_checksum_registers_buffer;


	inline void setBit(MappedRegisterPtr &a_register, const unsigned int bit_number);
	inline void clearBit(MappedRegisterPtr &a_register, const unsigned int bit_number);
	inline void setValue(MappedRegisterPtr &a_register, const uint64_t a_value);
protected:
	uint32_t          m_flags_buffer; // double buffering of the flags register
	bool              m_is_waiting;
	bool              m_is_done;
	bool              m_is_working_int;
	Data              m_data_transferred;

	void setFlag(const unsigned int bit_number, const unsigned int bit_value = 1);
	void clearFlag(const unsigned int bit_number);

	virtual void beforeReadMappedRegister() const;

	// override in subclasses; update the m_is_XXX fields AND CALL BASE LAST
	virtual void updateState() = 0;
public:

	// CONST
	const bool isWorkingInt() const {return m_is_working_int;};
	const bool isWaiting() const {return m_is_waiting;};
	const bool isDone() const {return m_is_done;};
	const bool isReallyWaiting() const;

	// MODIFIERS
	StreamStatusSync(const std::string &name, const uint32_t wait_latency, const uint32_t enable_checksum);
	virtual bool runCycle();
	virtual void reset();
};


/*
 * PUSH probe
 *
 */

class PushStreamStatusSync :
	public StreamStatusSync,
	public PushSinkSync,
	public PushSourceSync
{
private:
	bool m_pushing;
	bool m_done_delayed;
	bool m_waiting_delayed;

protected:
	virtual void updateState();
public:
	// CONST
	virtual bool isPushInputStalled(const t_port_number) const;
	virtual bool isPushOutputDone(const t_port_number output_port) const;

	// MODIFIERS
	PushStreamStatusSync(const std::string &name, const uint32_t wait_latency, const uint32_t enable_checksum);
	virtual void pushInput(const t_port_number, const maxcompilersim::Data&);
	virtual void reset();
};


/*
 * PULL probe
 *
 */

class PullStreamStatusSync :
	public StreamStatusSync,
	public PullSinkSync,
	public PullSourceSync
{
private:
	bool m_pulling;
	bool m_done_delayed;
	bool m_waiting_delayed;

protected:
	virtual void updateState();

public:
	// CONST
	virtual const Data& peekOutput(const t_port_number) const;
	virtual bool isPullOutputEmpty(const t_port_number) const;
	virtual bool isPullOutputDone(const t_port_number) const;

	// MODIFIERS
	PullStreamStatusSync(const std::string &name, const uint32_t wait_latency, const uint32_t enable_checksum);
	virtual void pullOutput(const t_port_number);
	virtual void reset();
};


} // namespace

#endif /* STREAMSTATUS_H_ */
