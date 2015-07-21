#ifndef MEMORYCONTROLLERRAM_H_
#define MEMORYCONTROLLERRAM_H_

#include <list>
#include <boost/smart_ptr/shared_ptr.hpp>
#include "Data.h"


namespace maxcompilersim {

/*
 * MemoryControllerRAMInterface is a simple abstract class that defines the interface
 * of the physical RAM. There are two methods available, read and write.
 * Notice that the addressing is done in bursts! Also notice that write
 * does not need the burst_size parameter, as burst_data also has a getSize method.
 * That said, it is easily possible for burst_data to not have the correct burst size,
 * hence burst_size was included as extra sanity check for the data to be written.
 *
 * MemoryControllerRAM takes the interface and implements it via MemoryMapped Files.
 */

class MemoryControllerRAMInterface
{
public:
	virtual ~MemoryControllerRAMInterface() {}

	// CONST
	virtual const Data read(const uint64_t burst_address, const size_t burst_size) const = 0;

	// MODIFIERS
	virtual void write(const uint64_t burst_address, /*see comment*/ const size_t burst_size, const Data burst_data) = 0;
};

typedef boost::shared_ptr<MemoryControllerRAMInterface> MemoryControllerRAMInterfacePtr;


/*
 * Wrapper around a mmap reserved area.
 * DOES NOT SUPPORT CONCURRENT READ/WRITES!
 */

class MemoryControllerRAM:
	public MemoryControllerRAMInterface
{
private:
	char *m_ram;
	uint64_t m_ram_size;
	size_t m_burst_size_bits;
	int m_fd;
	bool m_got_ram;
	std::string m_file_name;
	bool m_destructor_running;

protected:
	void UnlinkRAMFile();

public:
	MemoryControllerRAM(const uint64_t ram_size_bytes, const size_t burst_size_bits, const std::string mmap_file_name);
	MemoryControllerRAM(const MemoryControllerRAM& toCopy);
	virtual ~MemoryControllerRAM();

	// CONST
	virtual const Data read(const uint64_t burst_address, const size_t burst_size) const;

	// MODIFIERS
	virtual void write(const uint64_t burst_address, const size_t burst_size, const Data burst_data);

	// OPERATORS
	MemoryControllerRAM operator= (const MemoryControllerRAM& );
};

typedef boost::shared_ptr<MemoryControllerRAM> MemoryControllerRAMPtr;

} // namespace
#endif /* MEMORYCONTROLLERRAM_H_ */




