#ifndef DUALASPECTREG_H_
#define DUALASPECTREG_H_

#include "ManagerSync.h"
#include "PullSync.h"
#include "PushSync.h"

namespace maxcompilersim {

class DualAspectRegSync :
	public PullSinkSync,
	public PullSourceSync
{
	const size_t m_input_width;
	const size_t m_multiple;
	const t_port_number m_input_port;
	const t_port_number m_output_port;
	size_t m_size;
	Data m_data;

public:
	explicit DualAspectRegSync(
		const std::string &name,
		size_t input_width = 8,
		size_t output_multiple = 4);

	virtual void pullOutput(const t_port_number port_number);
	virtual const Data &peekOutput(const t_port_number port_number) const;
	virtual bool isPullOutputEmpty(const t_port_number port_number) const;
	virtual void reset();
	virtual bool runCycle();
};

} // namespace maxcompilersim

#endif // DUALASPECTREG_H_
