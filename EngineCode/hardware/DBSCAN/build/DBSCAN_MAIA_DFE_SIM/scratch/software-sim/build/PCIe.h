#ifndef PCIE_H_
#define PCIE_H_

#include <memory>

#include <cstdarg>
#include <stdint.h>

#include "Data.h"
#include "ManagerSync.h"
#include "PushSync.h"
#include "PullSync.h"
#include "SharedFIFO.h"

namespace maxcompilersim {

class PCIeBlockSync : public virtual ManagerBlockSync
{
protected:
	std::auto_ptr<SharedFIFO> m_fifo;

public:
	PCIeBlockSync() : ManagerBlockSync("") { }
	virtual ~PCIeBlockSync() { }

	virtual void createFIFO(const std::string &sim_prefix, int device_id, int stream_id, bool to_host);

	virtual void reset();
	virtual void abort();
};



/// PCIe data source node
class PCIePushSourceSync : public PCIeBlockSync, public PushSourceSync
{
protected:
	uint64_t m_data_count;

public:
	explicit PCIePushSourceSync(const std::string &name);
	virtual ~PCIePushSourceSync() {}

	virtual bool runCycle();
};

class PCIePullSourceSyncMax3 : public PCIeBlockSync, public PullSourceSync
{
protected:
	uint64_t m_data_count;
	Data m_currentWord;
	bool m_currentWordValid;

	virtual const Data &peekOutput(t_port_number output_port) const;
	virtual void pullOutput(t_port_number output_port);
public:
	explicit PCIePullSourceSyncMax3(const std::string &name);
	virtual ~PCIePullSourceSyncMax3() {}

	virtual bool isPullOutputEmpty(t_port_number output_port) const;
	virtual bool isPullOutputDone(t_port_number output_port) const;
	virtual bool runCycle();
};

class PCIePushSourceSyncMax4 : public PCIeBlockSync, public PushSourceSync
{
protected:
	uint64_t m_data_count;

public:
	explicit PCIePushSourceSyncMax4(const std::string &name);
	virtual ~PCIePushSourceSyncMax4() {}

	virtual bool runCycle();
};


/// PCIe data sink node
class PCIePushSinkSync : public PCIeBlockSync, public PushSinkSync
{
public:
	explicit PCIePushSinkSync(const std::string &name);
	virtual ~PCIePushSinkSync() {}

	virtual void pushInput(const t_port_number port_number, const Data &data);
	virtual bool isPushInputStalled(const t_port_number port_number) const;
	virtual bool runCycle() { return false; }
};

class PCIePushSinkSyncMax3 : public PCIePushSinkSync
{
public:
	explicit PCIePushSinkSyncMax3(const std::string &name);
	virtual ~PCIePushSinkSyncMax3() {}

	virtual void pushInput(const t_port_number port_number, const Data &data);
	virtual bool isPushInputStalled(const t_port_number port_number) const;
	virtual bool runCycle() { return false; }
};

} // namespace maxcompilersim

#endif // PCIE_H_
