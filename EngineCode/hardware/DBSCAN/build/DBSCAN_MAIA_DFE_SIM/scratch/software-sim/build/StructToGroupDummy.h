#ifndef STRUCT_TO_GROUP_DUMMY_H
#define STRUCT_TO_GROUP_DUMMY_H

#include "ManagerSync.h"
#include "PushSync.h"
#include "PullSync.h"
#include "Data.h"

namespace maxcompilersim {

class StructToGroupPushDummy :
	public PushSinkSync,
	public PushSourceSync
{
	uint m_width;
	t_port_number m_output_port, m_input_port;

public:
	StructToGroupPushDummy(const std::string &name, const uint width);

	virtual void pushInput(const t_port_number port_number, const Data &data);
	virtual bool isPushInputStalled(const t_port_number port_number) const;

	virtual bool runCycle() {
		return false;
	};
	virtual void reset() {};
};

#if 0
class StructToGroupPullDummy :
	public PullSinkSync,
	public PullSourceSync
{
	uint m_width;
	t_port_number m_output_port, m_input_port;

public:
	StructToGroupPullDummy(const std::string &name, const uint width);

	virtual void pullOutput(const t_port_number port_number);
	virtual const Data &peekOutput(const t_port_number port_number) const;
	virtual bool isPullOutputEmpty(const t_port_number port_number) const;

	virtual bool runCycle() {};
	virtual void reset() {};
};
#endif

} // namespace maxcompilersim

#endif // STRUCT_TO_GROUP_DUMMY_H
