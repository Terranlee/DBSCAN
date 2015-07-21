#ifndef SHARED_FIFO_H
#define SHARED_FIFO_H

#include <string>

#include <cstddef>
#include <cstring>

#include "max_shared_fifo.h"
#include "SimException.h"



namespace maxcompilersim {

class SharedFIFO
{
private:
	const std::string m_name;
	max_shared_fifo_t m_fifo;

public:
	SharedFIFO(const std::string &name, size_t size);
	~SharedFIFO();

	size_t getDataAvailable() const;
	size_t getSpaceAvailable() const;

	void read(void *data, size_t size);
	void write(const void *data, size_t size);

	void abort();
	void reset();
};

}

#endif // !defined(SHARED_FIFO_H)
