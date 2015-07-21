#ifndef DEMUX_H
#define DEMUX_H

#include <map>
#include <string>

#include "ManagerSync.h"
#include "PushSync.h"
#include "Data.h"
#include "MappedElements.h"

namespace maxcompilersim {

class DemuxSync :
	public PushSinkSync,
	public PushSourceSync
{
	std::map<int, t_port_number> m_output_map;
	int m_next_mux_output_n;

	MappedRegisterPtr m_reg;
	Data m_data;
	bool m_full;

public:
	DemuxSync(const std::string &name, const uint reg_width, const uint width = 32);
	DemuxSync &addOutput(const std::string &output_name);
	virtual void pushInput(const t_port_number port_number, const Data &data);
	virtual bool isPushInputStalled(const t_port_number port_number) const;
	virtual bool runCycle();
	virtual void reset();
};

} // namespace maxcompilersim

#endif // DEMUX_H
