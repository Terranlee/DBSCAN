#ifndef FILEPUSHSINK_H_
#define FILEPUSHSINK_H_

#include <fstream>
#include <string>

#include "Data.h"
#include "ManagerInfrastructure.h"

namespace maxcompilersim {

class FilePushSink :
	public PushSinkSync
{
private:
	const std::string m_filename;
	std::ofstream m_file;
	const uint m_bitwidth;
	const t_port_number m_input_port;
	const int m_terminationCount;
	int m_itemCount;

public:
	FilePushSink(const std::string &name, const std::string &filename, uint bitwidth, const int terminationCount = -1);

	virtual ~FilePushSink() {}

	virtual void pushInput(const t_port_number, const Data &data);
	virtual bool isPushInputStalled(const t_port_number) const;

	virtual bool runCycle() { return false; }
	virtual void reset();

	virtual bool isDone();
	bool isTerminationCountReached() const;
	bool isTerminationCountSet() const {return m_terminationCount != -1;}
};

} // namespace maxcompilersim

#endif /* FILEPUSHSINK_H_ */
