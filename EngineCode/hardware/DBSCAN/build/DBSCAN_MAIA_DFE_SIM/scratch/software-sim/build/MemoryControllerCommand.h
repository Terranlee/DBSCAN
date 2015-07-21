
#ifndef MEMORYCONTROLLERCOMMAND_H_
#define MEMORYCONTROLLERCOMMAND_H_

#include "Data.h"					// defines Data
#include <vector>
#include <boost/smart_ptr/shared_ptr.hpp>

namespace maxcompilersim {

#define STD_MEMORY_COMMAND_ADDRESS_SIZE 27
#define STD_MEMORY_COMMAND_INCREMENT_SIZE 8

/*
 * Enum for the Cmd mode: If command size is 0, then
 * the increment field gets re-interpreted as commands:
 */
#define MEMORY_COMMAND_CMD_SIZE 3
enum Command {
	CmdNOP                    = 0b000,
	CmdClearFlags             = 0b001,
	CmdSetFlags               = 0b010,
	CmdUNDEF1                 = 0b011,
	CmdBlockUntilFlagsSet     = 0b100,
	CmdBlockUntilFlagsCleared = 0b101,
	CmdUNDEF2                 = 0b110,
	CmdUNDEF3                 = 0b111
};

struct PortDescription
{
public:
	std::string m_name;
	int m_pin_from; // incl.
	int m_width;   // incl.

	PortDescription(std::string aName, int pin_from, int width):
		m_name		(aName),
		m_pin_from	(pin_from),
		m_width		(width)
	{}
};

/*
 * Class that encapsulates the memory commands (or properties thereof).
 * Objects are automatically created by the memory controller config - get hold of the obj by
 * calling getMemoryCommand() on the MemoryControllerConfig object.
 */

class MemoryControllerCommand
{
	// Sets the size of the read/write command count
	// Command count has a range of 1 to 2^N-1 'bursts'
	uint32_t		m_mem_cmd_count_size;		 // nominally set to <= log2(m_stream_fifo_depth)-1 and <=8
	uint32_t		m_mem_cmd_count_pad_size;	 // no bits needed for padding count size (if any)

	// Size of the read/write command start address
	// Command start addresses are in units of DIMM 'bursts'
	// A burst is 4 64bit reads/write on each DIMM
	uint32_t		m_mem_cmd_start_addr_size;	 // command addr range (bursts) 2^27 = 4G DDR2, 8G DDR3
	uint32_t		m_mem_cmd_pad_addr_size;	 // no. bits needed for padding (if any)

	// Sets the size of the read/write command increment
	// Command address increments by 1 to 2^N-1 'bursts' repeated for 'length' times
	uint32_t		m_mem_cmd_increment_size;	// 8
protected:
	friend class MemoryControllerConfig;
	explicit MemoryControllerCommand(
			uint32_t command_count_size,
			uint32_t command_address_size = STD_MEMORY_COMMAND_ADDRESS_SIZE,
			uint32_t increment_size = STD_MEMORY_COMMAND_INCREMENT_SIZE);
public:

	// CONST
	const uint32_t getAddressSize() const {return m_mem_cmd_start_addr_size;}
	const uint32_t getPadAddressSize() const {return m_mem_cmd_pad_addr_size;}
	const uint32_t getCountSize() const {return m_mem_cmd_count_size;}
	const uint32_t getPadCountSize() const {return m_mem_cmd_count_pad_size;}
	const uint32_t getIncrementSize() const {return m_mem_cmd_increment_size;}
	const uint32_t getStreamSelectSize(const uint8_t number_of_streams) const {
		return getStreamSelectSizeAndPad(number_of_streams).first;
	}
	const uint32_t getPadStreamSelectSize(const uint8_t number_of_streams) const {
		return getStreamSelectSizeAndPad(number_of_streams).second;
	}
	const std::pair<uint32_t,uint32_t> getStreamSelectSizeAndPad(const uint8_t number_of_streams) const;
	const uint32_t getTagSize() const;

	// STATIC
	static const uint32_t getAddress(const Data &command);
	static const uint32_t getNumberOfBursts(const Data &command);
	static const uint64_t getIncrement(const Data &command);
	static const uint64_t getIncrementAndMode(const Data &command);
	static const uint32_t getStreamSelect(const Data &command);
	static const uint32_t getStreamSelectOneHotValue(const Data &command);
	static const uint32_t getTag(const Data &command);
	static const uint32_t getCommandWidth();
	static const bool     isNullCommandNoStream(const Data &command) {return getStreamSelectOneHotValue(command) == 0;}
	static const bool     isCmdMode(const Data &command) {return getNumberOfBursts(command) == 0;}
	static const uint32_t getFlags(const Data &command);
	static const Command  getCmd(const Data &command);


	// for printing
	static std::vector<PortDescription> getOutputStreamFormatDescription(int stream_number);
	static void print(const Data &command);
};

typedef boost::shared_ptr<MemoryControllerCommand> MemoryControllerCommandPtr;

} // namespace

#endif /* MEMORYCONTROLLERCOMMAND_H_ */
