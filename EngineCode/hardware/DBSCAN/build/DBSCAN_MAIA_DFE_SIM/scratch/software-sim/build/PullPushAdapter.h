#ifndef PULLPUSHADAPTER_H_
#define PULLPUSHADAPTER_H_

#include "ManagerSync.h"
#include "PullSync.h"
#include "PushSync.h"

namespace maxcompilersim {

class PullToPushAdapterSync :
	public PullSinkSync,
	public PushSourceSync
{
	Data m_data;
	bool m_full;
	const t_port_number m_output_port;
	const t_port_number m_input_port;

public:
	explicit PullToPushAdapterSync(const std::string &name);
	virtual bool runCycle();
	virtual void reset();
};

} // namespace maxcompilersim

#endif // PULLPUSHADAPTER_H_
