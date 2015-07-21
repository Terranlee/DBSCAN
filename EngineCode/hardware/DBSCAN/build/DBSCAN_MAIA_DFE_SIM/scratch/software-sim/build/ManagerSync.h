#ifndef MANAGER_SYNC_H_
#define MANAGER_SYNC_H_

#include <boost/exception_ptr.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#include <boost/foreach.hpp>
#include <vector>
#include <string>

#include "ManagerBlock.h"


namespace maxcompilersim {


class ManagerDesignSync; //forward

/* Base class to all components that are active parts
 * of the simulation.
 */
class ManagerBlockSync : public ManagerBlock {
protected:
	ManagerDesignSync *m_manager_design;
public:
	ManagerDesignSync *getManagerDesignSync() const {return m_manager_design;}

	virtual void setManagerDesign(ManagerDesignSync *const manager_design) {
		m_manager_design = manager_design;
	}

	virtual ~ManagerBlockSync() {}

	virtual bool runCycle() = 0;

protected:
	explicit ManagerBlockSync(const std::string &name);
};


/*
 * Currently, one block within the simulation is allowed
 * to raise an interrupt. (See MemoryControllerPro.)
 * This class needs to use ManagerBlockIRQSync as a base
 * class. All the other blocks should use the normal
 * ManagerBlockSync.
 */
class ManagerBlockIrqSync : public ManagerBlockSync {
private:
	bool m_interrupt_signaled;
	boost::mutex m_interrupt_mutex;
	boost::condition_variable m_interrupt_cond_var;

public:
	virtual ~ManagerBlockIrqSync() {}

	virtual void reset() = 0; // must be called by the subclass!

	void waitForInterrupt(const void* connectionContext, int (*connectionDropped)(const void* connectionContext));

protected:
	explicit ManagerBlockIrqSync(const std::string &name);

	void signalInterrupt();
};



class WatchdogSync : public ManagerBlockSync {
	const uint64_t m_cycle_limit;
	uint64_t m_cycle;

public:
	WatchdogSync(const std::string &name, uint64_t limit);
	virtual ~WatchdogSync() {}

	virtual void reset();
	virtual bool runCycle();
};


/*
 * The class ManagerDesignSync is a "container" that holds
 * all the blocks (i.e., all kernels, memory manager, watches...)
 * Each of these blocks has a pointer back, so this class
 * also is a natural candidate for data that needs to be
 * shared globally among all blocks.
 *
 * As an example, ManagerDesignSync manages the default output
 * streams and might be used in future for setting up a map
 * to intern strings. (String <-> Int map)
 */

class ManagerDesignSync {
	friend class ManagerDesignRunnerSync;
private:
	std::vector<ManagerBlockSync*> m_blocks;
	ManagerBlockIrqSync *m_interrupt_source;  // points to the one IRQ source (if any)

	boost::shared_ptr<std::ostream> m_default_debug_output_stream;
	boost::shared_ptr<std::ostream> m_default_debug_output_backup;
	bool m_use_debug_backup;

public:
	// CONST
	void assignMappedRegisterAddress(uint32_t address, const std::string &name) const;
	void assignMappedMemoryAddress(uint32_t address, const std::string &name) const;
	std::ostream& getDefaultDebugOStream() const {return *m_default_debug_output_stream;}
	std::ostream& getDebugBackupOStream() const  {return *m_default_debug_output_backup;}
	bool useDebugBackup() const {return m_use_debug_backup;}


	// MODIFIERS
	ManagerDesignSync();
	void addBlock(ManagerBlockSync &block);

	void redirectDefaultDebugStream(const std::string &file_name);
	void redirectDefaultDebugStream(int fd);
	void redirectDebugBackup(const std::string &file_name);


	void setSimParameter(const std::string &name, const std::string &value);
	const std::string getSimParameter(const std::string &name);


	// OPERATORS
	ManagerDesignSync & operator*() {return *this;}


	// TEMPLATED
	template<class T>
	std::vector<T*> getBlocksOfType() {
		std::vector<T*> found_blocks;

		BOOST_FOREACH(ManagerBlockSync *block, m_blocks) {
			T *casted_block = dynamic_cast<T*>(block);
			if(casted_block)
				found_blocks.push_back(casted_block);
		}

		return found_blocks;
	}
};


/*
 * Manager Design Runner implements the logic needed to
 * run a simulation. It relies on the ManagerDesign to
 * do that.
 * Notice that Manager Design Runner also uses some of
 * the global data structures found in ManagerDesign,
 * as, e.g., the standard output stream.
 */

class ManagerDesignRunnerSync {
private:
	ManagerDesignSync &m_design;
	boost::thread m_thread;

	mutable boost::mutex m_abort_mutex;
	mutable boost::mutex m_start_stop_mutex;
	boost::condition_variable m_abort_cond_var;
	boost::exception_ptr m_exception;
	bool m_thread_running;
	bool m_shutting_down;

public:
	explicit ManagerDesignRunnerSync(ManagerDesignSync &design);

	~ManagerDesignRunnerSync();

	uint32_t getMappedElement(uint32_t address);
	void setMappedElement(uint32_t address, uint32_t value, uint mask);
	void mecWriteMappedRegisters(uint32_t address,
									const mapped_registers_data_t *data,
									size_t item_count);
	void mecReadMappedRegisters(uint32_t address,
								mapped_registers_data_t *data,
								size_t item_count);
	void mecWriteMappedMemories(uint32_t address,
								const mapped_memories_data_t *data,
								size_t item_count);
	void mecReadMappedMemories(uint32_t address,
								mapped_memories_data_t *data,
								size_t item_count);
	void reset();
	void waitAbort();
	std::string getAbortReason() const;
	std::string *tryGetAbortReason() const;
	bool aborted() const;

	bool waitForInterrupt(const void* connectionContext, int (*connectionDropped)(const void* connectionContext));

	void redirectDefaultDebugStream(const std::string &file_name);
	void redirectDefaultDebugStream(int fd);
	void redirectDebugBackup(const std::string &file_name);
	void setDumpDirectory(const std::string &dir_name);
	void flushDebugStreams();
	void setSimParameter(const std::string &name, const std::string &value);
	const std::string getSimParameter(const std::string &name);

	void stopThread();
	void startThread();
	void shutdown();
	void abortConnections();
private:
	void threadLoop();

	void assertNotAborted() const;
};

} // namespace maxcompilersim

#endif // MANAGER_SYNC_H_
