#ifndef DUALASPECTMUX_H
#define DUALASPECTMUX_H

#include "ManagerSync.h"
#include "PushSync.h"
#include "PullSync.h"
#include "Data.h"
#include "MappedElements.h"

namespace maxcompilersim {

class DualAspectMuxSync :
	public PullSinkSync,
	public PushSourceSync
{
	const size_t m_output_width;
	const size_t m_multiple;
	size_t m_size;
	Data m_data;
	bool m_is_done;
	const t_port_number m_output_port;
	const t_port_number m_input_port;

protected:
	virtual bool isPushOutputDone(const t_port_number output_port) const;

public:
	explicit DualAspectMuxSync(
		const std::string &name,
		size_t output_width,
		size_t input_multiple);
	virtual bool runCycle();
	virtual void reset();

};

} // namespace maxcompilersim

#endif // DUALASPECTMUX_H
