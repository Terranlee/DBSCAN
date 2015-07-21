#ifndef KERNELMANAGERBLOCKSYNC_H_
#define KERNELMANAGERBLOCKSYNC_H_

//#define TRACE_KERNEL_MANAGER_BLOCK


#ifdef TRACE_KERNEL_MANAGER_BLOCK
#include <iostream>
#endif

#include <boost/shared_ptr.hpp>

#include <map>
#include <string>
#include <fstream>

#include <stdint.h>

#include "ManagerSync.h"
#include "PullSync.h"
#include "PushSync.h"
#include "Debuggable.h"
#include "varint_u.h"

namespace maxcompilersim {

typedef enum {reg_dbg_stall_vector = 0, reg_dbg_ctld_empty = 1, reg_dbg_ctld_almost_empty = 2,
	reg_dbg_ctld_done = 3, reg_dbg_ctld_read = 4, reg_dbg_done_out = 5, reg_dbg_full_level = 6,
	reg_dbg_flush_start_level = 7, reg_dbg_fill_level = 8, reg_dbg_flush_level = 9, reg_dbg_ctld_request = 10,
	reg_dbg_ctld_read_pipe_dbg = 11, reg_dbg_flush_start = 12, reg_dbg_flushing = 13, reg_dbg_out_valid = 14,
	reg_dbg_out_stall = 15, reg_dbg_number = 16} EnumDebugRegisters;



class KernelManagerBlockSync :
	public PullSinkSync,
	public PushSourceSync,
	public Debuggable
{
private:
	class OutputValue {
	public:
		Data m_data;
		bool m_valid;
		OutputValue();
	};

	class Input{
	private:
		const uint32_t m_latency;
		uint32_t       m_current_read_pos;
		uint32_t       m_current_write_pos;
		const uint16_t m_id;
		const long&    m_cycle;
		bool*          m_willReadQueue;
		bool           m_doneReading;

		const Input& operator=(const Input& rhs) { assert(false); /*not possible*/ }

		void clearReadRequestQueue() {
			for (int i = 0; i < m_latency + 1; i++)
				m_willReadQueue[i] = false;
		}
	public:
		Input(uint16_t id, const long& cycle_counter, uint32_t latency):
			m_latency           (latency),
			m_current_read_pos  (0),
			m_current_write_pos (0),
			m_id                (id),
			m_cycle             (cycle_counter),
			m_willReadQueue     (new bool[latency + 1]),
			m_doneReading       (false)
		{
			clearReadRequestQueue();
			update();
		}

		virtual ~Input() {
			delete [] m_willReadQueue;
		}

		void queueRead(bool willRead) {
			m_willReadQueue[m_current_write_pos] = willRead;
		}

		void reset() {
			m_doneReading = false;
			clearReadRequestQueue();
		}

		void update() {
#ifdef TRACE_KERNEL_MANAGER_BLOCK
			std::cout << "Current readrequest buffer " << m_id << ": ";
			for (int i = 0 ; i < m_latency; i++) {
				if (m_current_write_pos == i) std::cout << "w";
				if (m_current_read_pos == i) std::cout << "r";
				std::cout << m_willReadQueue[i] << ", ";
			}
			std::cout << std::endl;
#endif
			m_current_write_pos = (m_cycle + m_latency - 1) % m_latency;
			m_current_read_pos  = m_cycle % m_latency;
		}


		uint16_t id() const { return m_id; }
		bool wantToRead() const
		{
			if (m_doneReading)
				return false;

			return m_willReadQueue[m_current_read_pos];
		}

		void markAsDone() {
			m_doneReading = true;
		}
	};
	typedef boost::shared_ptr<Input> InputPtr;

	// State-machine constants
	const int  m_num_photon_state_machines;
	const long m_fill_level_max;
	const long m_warm_up;
	const long m_control_pipelining;

	// State-machine variables
	std::map<t_port_number, OutputValue> m_output_values;
	typedef std::map<t_port_number, InputPtr> InputMap;
	typedef std::map<t_port_number, unsigned int> OutputMap;

	InputMap m_input_numbering;
	OutputMap m_output_numbering;
	long m_cycle;
	long m_fill_level;
	long m_flush_level;
	bool m_flushing_active;
	int  m_flush_level_start;

	const std::string m_primary_input_string;
	t_port_number m_primary_input_port;

	int m_sim_progress_output_frequency;
	bool m_javasim_mode;

	// double buffering of debug information
	uint64_t m_in_empty_buffer;
	uint64_t m_in_done_buffer;
	uint64_t m_in_read_buffer;
	uint64_t m_out_valid_buffer;
	uint64_t m_out_stall_buffer;
	long m_fill_level_buffer;
	long m_flush_level_buffer;
	int m_flush_start_level_buffer;
	bool m_done_out_buffer;
	bool m_flushing_active_buffer;


protected:
	MappedRegisterPtr m_dbg_regs [reg_dbg_number];
	uint32_t m_dbg_regs_size [reg_dbg_number];

	virtual void beforeReadMappedRegister() const;
	void setDebugRegisterValue(const EnumDebugRegisters debug_reg, long value) const;

public:
	KernelManagerBlockSync(
		const std::string &name,
		long fill_level_max,
		long control_pipelining,
		long constant_latency_max,
		int sim_progress_output_frequency,
		const std::string &primary_input,
		int num_photon_state_machines);

	virtual ~KernelManagerBlockSync() {}

	void enableJavaSimMode();

	void setupMappedMemory(
		const std::string &name,
		uint bit_width,
		uint depth);

	void setupMappedRegister(const std::string &name, uint bitwidth);
	void addDebugRegister(const EnumDebugRegisters debug_reg, uint bitwidth, uint initial_value);

	virtual void reset();

	virtual bool runCycle();

	virtual bool isPushOutputDone(const t_port_number) const;

	/*
	 * These methods are public so regs/mems can be set-up by the
	 * Java driven simulation wrapper by name without having
	 * to worry about addresses.
	 */

	template<typename T>
	T getMappedRegValue(const std::string &name) {
		MappedRegisterPtr reg(getMappedReg(name, T::totalBitSize));
#ifdef TRACE_KERNEL_MANAGER_BLOCK
		T d(varint_u<T::totalBitSize>(reg->getData()));
		std::cout << "Kernel got value for mapped reg " << name << " as " << d << std::endl;
#endif

		return T(varint_u<T::totalBitSize>(reg->getData().toVarIntU<T::totalBitSize>()));
	}

	template<typename T>
	void setMappedRegValue(const std::string &name, const T &value) {
		MappedRegisterPtr reg(getMappedReg(name, T::totalBitSize));
		reg->setData(Data(value.getBitString()));
	}

	template<uint size>
	void setMappedRegValue(const std::string &name, const varint_u<size> &value) {
		MappedRegisterPtr reg(getMappedReg(name, size));
		reg->setData(Data(value));
	}

	template<uint size>
	void setMappedRegValue(const std::string &name, const varint_s<size> &value) {
		MappedRegisterPtr reg(getMappedReg(name, size));
		reg->setData(Data(varint_u<size>(value)));
	}

	template<typename T>
	T getMappedMemValue(const std::string &name, const uint entry) {
		AbstractMappedMemoryPtr mem(getMappedMem(name, T::totalBitSize));

		return T(varint_u<T::totalBitSize>(mem->getData(entry).toVarIntU<T::totalBitSize>()));
	}

	template<typename T>
	void setMappedMemValue(const std::string &name, const uint entry, const T &value) {
		AbstractMappedMemoryPtr mem(getMappedMem(name, T::totalBitSize));
		mem->setData(entry, Data(value.getBitString()));
	}

protected:
	bool m_needToStartFlushingInNextCycle;
	bool m_mappedElementsChanged;

	virtual void mappedElementChanged(const MappedElement &changedElement) {
		m_mappedElementsChanged = true;
	};

	t_port_number registerInput(const std::string &name, uint16_t id, uint32_t latency);
	t_port_number registerOutput(const std::string &name, uint16_t id);
	void startFlushing();
	void startFlushingAndOffsetFlushLevel(int value);
	void startFlushingNextCycle() {m_needToStartFlushingInNextCycle = true;}
	void updateFlushing() {if (m_needToStartFlushingInNextCycle) {m_needToStartFlushingInNextCycle = false; startFlushing();} }
	void queueReadRequest(const t_port_number input_port, bool willRead);

	void markInputDone(const t_port_number input_port);

	bool needsToReadInput(t_port_number input_port) const;
	inline long getCycle() const { return m_cycle; }
	inline long getFillLevel() const {return m_fill_level; }
	inline long getFlushLevel() const {return m_flush_level;}
	inline bool isFlushingActive() const {return m_flushing_active;}
	void flushReset();


	template<typename T>
	void setRom(uint64_t raw_data[], T cooked_data[], uint width, uint depth) {
		uint chunks_per_word = varint_u<T::totalBitSize>::e_num_chunks;
		for(uint i = 0; i < depth; i++)
			cooked_data[i] =
				T(
					varint_u<T::totalBitSize>(
							&raw_data[i*chunks_per_word],
							chunks_per_word));
	}

	template<typename T>
	T readInput(const t_port_number input_port) {
		const Data &data(pullInput(input_port));
#ifdef TRACE_KERNEL_MANAGER_BLOCK
		std::cout << "Kernel " << getName() << " reading " << data << " on input " << getPullInputName(input_port) << std::endl;
#endif
		return T(data.toVarIntU<T::totalBitSize>());
	}

	template<typename T>
	void writeOutput(const t_port_number output_port, const T &value) {
#ifdef TRACE_KERNEL_MANAGER_BLOCK
		std::cout << "Kernel " << getName() << " outputting " << value << " on output " << getPushOutputName(output_port) << std::endl;
#endif
		m_output_values[output_port].m_data = DynamicVarUInt(value.getBitString());
		m_output_values[output_port].m_valid = true;
	}

	void simPrintf(const char *stream_name, int seq, const boost::format &message) {
		getDebugStreams()->write(stream_name, seq, message.str());
	}

	void simPrintf(const char *stream_name, int seq, const std::string &message) {
		getDebugStreams()->write(stream_name, seq, message);
	}

	virtual void runComputationCycle() = 0;

	// When this is called the state-machine state is undefined.
	virtual void resetComputation()    = 0;
	// Called when the kernel does a reset after a flush
	virtual void resetComputationAfterFlush() = 0;

	// This will always be called after a resetComputation()
	virtual int  getFlushLevelStart()  = 0;

private:
	void printDebugRegister() const;
	bool isPrimaryInput(const t_port_number input_port) const;
	bool ceForThisCycle() const;
	bool runStateMachinePreCompute();
	void runStateMachinePostCompute();
	void resetStateMachine();
	void flushResetStateMachine();
	MappedRegisterPtr getMappedReg(const std::string &name, uint bitwidth);
	AbstractMappedMemoryPtr getMappedMem(const std::string &name, uint bitwidth);
};

} // namespace maxcompilersim

#endif /* KERNELMANAGERBLOCKSYNC_H_ */
