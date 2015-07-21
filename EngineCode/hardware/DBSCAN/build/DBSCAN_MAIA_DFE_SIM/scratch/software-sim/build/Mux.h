#ifndef MUX_H
#define MUX_H

#include "ManagerSync.h"
#include "PushSync.h"
#include "Data.h"
#include "MappedElements.h"

#include <map>
#include <tr1/unordered_map>
#include <string>

namespace maxcompilersim {

class MuxSync :
	public PushSinkSync,
	public PushSourceSync
{
	std::tr1::unordered_map<uint32_t, int> m_input_map;
	int m_next_mux_input_n;

	MappedRegisterPtr m_reg;
	Data m_data;
	bool m_full;

public:
	MuxSync(const std::string &name, const uint reg_width, const uint width = 32);
	MuxSync &addInput(const std::string &input_name);
	virtual void pushInput(const t_port_number port_number, const Data &data);
	virtual bool isPushInputStalled(const t_port_number port_number) const;
	virtual bool runCycle();
	virtual void reset();
};

} // namespace maxcompilersim

#endif // MUX_H
