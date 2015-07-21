#ifndef FANOUT_H
#define FANOUT_H

#include <map>
#include <string>

#include "ManagerSync.h"
#include "PushSync.h"
#include "Data.h"
#include "MappedElements.h"

namespace maxcompilersim {

/*
 * Fanout is like a demux (1:N). Difference to demux is that several output streams
 * can be enabled at the same time!
 *
 */

class FanoutSync :
	public PushSinkSync,
	public PushSourceSync
{
	std::map<int, t_port_number> m_output_map;
	int m_next_output_n;

	MappedRegisterPtr m_reg;
	Data m_data;
	bool m_full;

public:
	FanoutSync(const std::string &name, const uint reg_width, const uint width = 32);
	FanoutSync &addOutput(const std::string &output_name);
	virtual void pushInput(const t_port_number port_number, const Data &data);
	virtual bool isPushInputStalled(const t_port_number port_number) const;
	virtual bool runCycle();
	virtual void reset();
};

} // namespace maxcompilersim

#endif // FANOUT_H
