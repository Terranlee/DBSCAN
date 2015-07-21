#ifndef _SIM_TERMINATOR_H
#define _SIM_TERMINATOR_H

#include <string>
#include <vector>

#include "ManagerSync.h"
#include "MappedElements.h"
#include "FilePushSink.h"
#include "KernelManagerBlockSync.h"

namespace maxcompilersim {

class SimTerminator : public ManagerBlockSync {
private:
	std::vector<FilePushSink*> m_sinks_to_monitor;
	std::vector<KernelManagerBlockSync *> m_kernels_to_monitor;
	bool m_haveTerminationCount;

public:
	SimTerminator(const std::string &name);
	virtual ~SimTerminator() throw () {};

	void addKernel(KernelManagerBlockSync *kernel_to_monitor);
	void addFileSink(FilePushSink *sink_to_montior);

	void setHaveTerminationCount(bool value);

	virtual void reset() {}
	virtual bool runCycle();
};

} // namespace maxcompilersim

#endif // _SIM_TERMINATOR_H
