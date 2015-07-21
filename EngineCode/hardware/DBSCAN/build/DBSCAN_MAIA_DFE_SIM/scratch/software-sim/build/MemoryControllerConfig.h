
#ifndef MEMORYCONTROLLERCONFIG_H_
#define MEMORYCONTROLLERCONFIG_H_

#include "MemoryControllerCommand.h"

namespace maxcompilersim {

#define MAX_STREAM_FIFO_DEPTH_WORDS 512
#define MAX_COMMAND_FIFO_DEPTH_WORDS 32

// two types of boards
enum EnumBoardType {btMax2, btMax3, btMax4};
/* burst sizes:
    Max2       :         96 bytes;
    Max3 normal: 192 or 384 bytes;
    Max3 comp. :  48 or  96 bytes; (see extra flag)*/
enum EnumBurstSize {/*bs48,*/ bs96, bs192, bs384};
// arbitration mode (only round robin supported)
enum EnumArbitrationMode {amRoundRobin, amDynamic};
// do we have ecc/parity turned on?
enum EnumMemCheckLevel {mclNone = 0, mclParity = 1, mclEcc = 8};

/*
 * Both, address generator and memory controller need an object of this class for configuration.
 * Notice that MemoryControllerConfig autom. creates a MemoryControllerCommand object that
 * has information about the various bit-withs of the memory command. You have access to this
 * object via the getMemoryCommand() method. (In addition there are static methods in both classes.)
 *
 */
class MemoryControllerConfig;
typedef boost::shared_ptr<MemoryControllerConfig> MemoryControllerConfigPtr;

class MemoryControllerConfigParams {
public:
	const EnumBoardType       board_type;
	const EnumBurstSize       burst_size;
	const uint64_t            mem_size_bytes;
	const std::string         swap_file_name;
	const EnumArbitrationMode arb_mode;
	const uint32_t            engine_size;
	const EnumMemCheckLevel   mem_check_level;
	const bool                max2CompatMode;
	const bool                max4QuarterMode;
	const size_t              commandFifoWordDepth;
	const size_t              streamFifoWordDepth;

	MemoryControllerConfigParams(const EnumBoardType board_type,
			const EnumBurstSize burst_size,
			const uint64_t mem_size_bytes = 0,
			const std::string swap_file_name = "",
			const EnumArbitrationMode arb_mode = amRoundRobin,
			const uint32_t engine_size = 0,
			const EnumMemCheckLevel mem_check_level = mclNone,
			const bool max2CompatMode = false,
			const bool max4QuarterMode = false,
			const size_t commandFifoWordDepth = MAX_COMMAND_FIFO_DEPTH_WORDS,
			const size_t streamFifoWordDepth = MAX_STREAM_FIFO_DEPTH_WORDS)
	:
		board_type            (board_type),
		burst_size            (burst_size),
		mem_size_bytes        (mem_size_bytes),
		swap_file_name        (swap_file_name),
		arb_mode              (arb_mode),
		engine_size           (engine_size),
		mem_check_level       (mem_check_level),
		max2CompatMode        (max2CompatMode),
		max4QuarterMode       (max4QuarterMode),
		commandFifoWordDepth  (commandFifoWordDepth),
		streamFifoWordDepth   (streamFifoWordDepth)
	{}
};


class MemoryControllerConfig
{
protected:
	// internal configuration options (memoryctrl only)
	EnumBoardType   m_board_type;
	size_t          m_memory_bus_width;     // bits
	size_t          m_stream_fifo_depth;    // amount of words of m_memory_bus_width
	size_t          m_burst_word_size;      // amount of words of m_memory_bus_width
	size_t          m_command_fifo_depth;
	size_t          m_command_fifo_width;
	MemoryControllerCommandPtr m_command;   // all things related to a memory controller command
	std::string     m_swap_file_name;
	uint64_t        m_mem_size_bytes;
	uint32_t        m_ecc_parity_size_bits;
	EnumMemCheckLevel m_mem_check_level;
	bool            m_max2CompatMode;
	bool            m_max4QuarterMode;

	// STATIC
	static std::string createTempFile();
public:

	// CONST Methods
	const EnumBoardType getBoardType() const {return m_board_type;}
	const size_t getMemoryBusBitWidth() const {return m_memory_bus_width;}
	const size_t getStreamFIFOWordDepth() const {return m_stream_fifo_depth;}
	const size_t getStreamFIFOBitWidth() const {return getMemoryBusBitWidth();}

	const size_t getBurstWordSize() const {return m_burst_word_size;}
	const size_t getBurstBitSize() const {return getBurstWordSize() * getMemoryBusBitWidth();}

	const size_t getCommandFIFOWordDepth() const {return m_command_fifo_depth;}
	const size_t getCommandFIFOBitWidth() const {return m_command_fifo_width;}

	const uint64_t getMemoryByteSize() const {return m_mem_size_bytes;}
	const std::string &getSwapFileName() const {return m_swap_file_name;}

	const MemoryControllerCommandPtr getMemoryCommand() const {return m_command;}

	const uint32_t getMaxMemoryBurstWordSize() const;
	const uint32_t getMaxMemoryBusBitWidth() const;

	const uint32_t getEccParityEngineBitSize() const {return m_ecc_parity_size_bits;}
	const EnumMemCheckLevel getEccParityLevel() const {return m_mem_check_level;}

	const bool equals(const MemoryControllerConfigParams & params) const;

	// MODIFIERS
	MemoryControllerConfig(const MemoryControllerConfigParams & params);

	// STATIC
	static MemoryControllerConfigPtr getMax2Config(int number_words_per_burst, uint32_t eccParityEngineBitSize, uint32_t checkLevel, uint32_t cmdQSize);
	static MemoryControllerConfigPtr getMax3Config(int number_words_per_burst, uint32_t eccParityEngineBitSize, uint32_t checkLevel, uint32_t max2CompatMode, uint32_t cmdQSize);
	static MemoryControllerConfigPtr getMax4Config(int number_words_per_burst, uint32_t eccParityEngineBitSize, uint32_t checkLevel, uint32_t max2CompatMode, uint32_t quarterMode, uint32_t cmdQSize);

	static const uint32_t getNumberOfBitsToEncodeValue(const uint32_t number);
	static const uint32_t getMaxMemoryBurstWordSize(const EnumBoardType some_max_board);
	static const uint32_t getMaxMemoryBusBitWidth(const EnumBoardType some_max_board);
};

} // namespace

#endif /* MEMORYCONTROLLERCONFIG_H_ */

