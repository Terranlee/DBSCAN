
#ifndef MEMORYCONTROLLERSTREAMS_H_
#define MEMORYCONTROLLERSTREAMS_H_

#include "Fifos.h"
#include "MemoryControllerRAM.h"
#include "MemoryControllerConfig.h"
#include <boost/smart_ptr/shared_ptr.hpp>
#include <vector>
#include <deque>


namespace maxcompilersim {


typedef boost::shared_ptr<FifoPushToPullSync> FifoPushToPullSyncPtr;
typedef boost::shared_ptr<FifoPushToPushSync> FifoPushToPushSyncPtr;



/*
 * All input/output streams in a memory controller are backed by
 * FIFOs. Turns out all are PushToPull, but since this might
 * change, the FifoBackendStream class provides some common
 * ABSTRACT ancestor.
 *
 */
enum StreamType {Stream_Is_Input, Stream_Is_Output, Stream_Is_Command};
class FifoBackedStream
{
protected:
	std::string m_stream_name_input;
	std::string m_stream_name_output;
	StreamType m_stream_type;
	bool m_done;

	FifoBackedStream (const std::string &stream_name_input, // name of push input
			const std::string &stream_name_output, // name of pull output
			const StreamType stream_type):
		m_stream_name_input (stream_name_input),
		m_stream_name_output(stream_name_output),
		m_stream_type       (stream_type),
		m_done              (false)
	{}
public:
	std::string getInputName() const {return m_stream_name_input;}
	std::string getOutputName() const {return m_stream_name_output;}
	StreamType getStreamType() const {return m_stream_type;}

	virtual ~FifoBackedStream() {}
	virtual Fifo &getFifo() = 0;
	virtual void reset() {m_done = false;};
	virtual void newCycle() {};
};


/*
 * FifoBackedPushToPullStream class extends FifoBackedStream and
 * implements a PushToPull Fifo with custom names for the input
 * and output streams.
 *
 */


struct FifoConfig {
	size_t width;
	size_t depth;

	FifoConfig (size_t awidth, size_t adepth):
		width	(awidth),
		depth	(adepth)
	{}
};

class FifoBackedPushToPullStream:
	public FifoBackedStream,
	public PushSinkSync,
	public PullSourceSync
{
protected:
	const t_port_number m_input_port;
	const t_port_number m_output_port;
	Fifo m_fifo;
	bool m_output_done;

	FifoBackedPushToPullStream(const std::string &stream_name_input,
		const std::string &stream_name_output,
		const StreamType afifotype,
		const FifoConfig &fifo_config);

	// taken from fifos.cpp
	virtual void pushInput(const t_port_number input_port, const Data &data);
	virtual bool isPushInputStalled(const t_port_number input_port) const;
	virtual void pullOutput(const t_port_number output_port);
	virtual const Data &peekOutput(const t_port_number output_port) const;
	virtual bool isPullOutputEmpty(const t_port_number output_port) const;
	virtual bool isPullOutputDone(const t_port_number output_port) const;
	virtual bool runCycle() { return false; } // passive operation

public:
	// CONST
	const size_t getNumberOfWordsFreeInFifo() const;
	const size_t getNumberOfWordsInFifo() const;
	const size_t getCapacity() const;
	bool isEmpty() const;
	bool isFull() const;
	const Data &peek() const;
	const t_port_number getInputPortNumber() const {return m_input_port;}
	const t_port_number getOutputPortNumber() const {return m_output_port;}

	// MODIFIERS
	void pop();
	virtual ~FifoBackedPushToPullStream() {}
	virtual void reset();
	virtual void newCycle();
	virtual Fifo &getFifo() {return m_fifo;}
};


/*
 * FifoCommandStream is directly derived from FifoBackedPushToPullStream.
 * It is used for all command channels within the different
 * memory control groups.
 *
 */

class FifoCommandStream:
	public FifoBackedPushToPullStream
{
protected:
	/*Our proxy is our friend...*/
	friend class MemoryProxyCommandSinkSync;
public:
	FifoCommandStream (const std::string &stream_name_input, const std::string &stream_name_output, const FifoConfig &fifo_config):
		ManagerBlockSync(stream_name_input),
		FifoBackedPushToPullStream(stream_name_input,
				stream_name_output, Stream_Is_Command, fifo_config)
	{}
};

typedef boost::shared_ptr<FifoCommandStream> FifoCommandStreamPtr;

/*
 * All input and output streams (other than command inputs) of the
 * memory manager have their own command queue assigned.
 * This is taken care of in InputOutputStream class below.
 * ABSTRACT CLASS
 */

class InputOutputStream:
	public FifoBackedPushToPullStream
{
protected:
	std::deque<Data> m_command_queue;
	size_t m_command_queue_size;
	size_t m_command_queue_width;
	bool   m_debug_flags;
	bool   m_debug_streams;

	InputOutputStream(const std::string &stream_name_input,
			const std::string &stream_name_output,
			const StreamType stream_type,
			const FifoConfig &data_fifo_config,
			const FifoConfig &command_fifo_config):
		ManagerBlockSync(stream_name_input),
		FifoBackedPushToPullStream(stream_name_input, stream_name_output,
				stream_type, data_fifo_config),
		m_command_queue         (std::deque<Data>()),
		m_command_queue_size    (command_fifo_config.depth),
		m_command_queue_width   (command_fifo_config.width),
		m_debug_flags           (getenv("MAX_MCP_FLAGS_DEBUG") != NULL),
		m_debug_streams         (getenv("MAX_MCP_MEMCTRSTREAM_DEBUG") != NULL)
		{};

	bool isNextFlagCommandValid(const uint32_t flagRegister, const Data& nextCommand) const;
	void executeFlagCommand(uint32_t &flagRegister, const Data& command) const;

public:
	// CONST
	bool isCommandQueueFull() const {return m_command_queue.size() >= m_command_queue_size;}
	bool isCommandQueueEmpty() const {return m_command_queue.size() == 0;}
	const Data getNextCommand() const;
	virtual bool isNextCommandValid(const uint32_t flagRegister, const MemoryControllerConfigPtr configuration) const = 0;

	// MODIFIERS
	void popCommandQueue() {m_command_queue.pop_front();}
	void addCommandToQueue (const Data someCommand) {m_command_queue.push_back(someCommand);}
	virtual bool executeCommand(uint32_t &flagRegister, MemoryControllerRAMInterfacePtr memory, const MemoryControllerConfigPtr configuration) = 0;
	virtual void reset();
	virtual ~InputOutputStream() {}
};

typedef boost::shared_ptr<InputOutputStream> InputOutputStreamPtr;


/*
 * All input streams are objects of the FifoInputStream class that is
 * a specialization of the InputOutputStream class.
 *
 */
class FifoInputStream:
	public InputOutputStream
{
protected:
	/*Our proxy is our friend...*/
	friend class MemoryProxyPushSinkSync;
public:
	FifoInputStream (const std::string &stream_name_input,
			const std::string &stream_name_output,
			const FifoConfig &data_fifo_config,
			const FifoConfig &command_fifo_config):
		ManagerBlockSync(stream_name_input),
		InputOutputStream(stream_name_input,
				stream_name_output, Stream_Is_Input, data_fifo_config, command_fifo_config)
	{}
	virtual bool isNextCommandValid(const uint32_t flagRegister, const MemoryControllerConfigPtr configuration) const;
	virtual bool executeCommand(uint32_t &flagRegister, MemoryControllerRAMInterfacePtr memory, const MemoryControllerConfigPtr configuration);
};
typedef boost::shared_ptr<FifoInputStream> FifoInputStreamPtr;



/*
 * All output streams are objects of the FifoOutputStream class that is
 * derived from InputOutputStream
 *
 */

class FifoOutputStream:
	public InputOutputStream
{
protected:
	/* The memory proxy class is our friend, since it is a proxy to this class.*/
	friend class MemoryProxyPullSourceSync;
public:
	FifoOutputStream (const std::string &stream_name_input,
			const std::string &stream_name_output,
			const FifoConfig &data_fifo_config,
			const FifoConfig &command_fifo_config):
		ManagerBlockSync(stream_name_input),
		InputOutputStream(stream_name_input,
				stream_name_output, Stream_Is_Output, data_fifo_config, command_fifo_config)
	{}
	virtual bool isNextCommandValid(const uint32_t flagRegister, const MemoryControllerConfigPtr configuration) const;
	virtual bool executeCommand(uint32_t &flagRegister, MemoryControllerRAMInterfacePtr memory, const MemoryControllerConfigPtr configuration);
};
typedef boost::shared_ptr<FifoOutputStream> FifoOutputStreamPtr;


}


#endif /* MEMORYCONTROLLERSTREAMS_H_ */
