#ifndef LOOPS_H_
#define LOOPS_H_

#include <string>

#include "Data.h"
#include "ManagerInfrastructure.h"

namespace maxcompilersim {

// Loop* nodes are not meant to do anything.
// They should be bypassed when connecting up
// manager blocks.
// Use Loop*SourceSync::getTarget() to get the
// node to which the loop ultimately connects.

// forward declarations
class LoopPushSinkSync;
class LoopPullSinkSync;

// Naming:
// LoopSource is the source of the loop
// i.e. a sink block which takes data
// and pushes it to the other side of the
// loop.
class LoopPushSourceSync :
	public PushSinkSync
{
private:
	PushSinkSync &m_target;
	const std::string m_target_port;

public:
	LoopPushSourceSync(
		const std::string &name,
		PushSinkSync &target,
		const std::string &target_port)
	:
		ManagerBlockSync(name),
		m_target(target),
		m_target_port(target_port)
	{
		registerPushInput(name);
	}

	virtual ~LoopPushSourceSync() {}

	PushSinkSync &getTarget() const
	{ return m_target; }

	const std::string &getTargetPort() const
	{ return m_target_port; }

	virtual void pushInput(const t_port_number port, const Data &)
	{ throw SimException("This method should never be called"); }

	virtual bool isPushInputStalled(const t_port_number port) const
	{ throw SimException("This method should never be called"); }

	virtual void reset() {};
	virtual bool runCycle() { return false; };
};



class LoopPushSinkSync :
	public PushSourceSync
{
public:
	explicit LoopPushSinkSync(const std::string &name) :
		ManagerBlockSync(name)
	{
		registerPushOutput(name);
	}

	virtual ~LoopPushSinkSync() {}

	virtual bool isPushOutputDone(const t_port_number port) const
	{ throw SimException("This method should never be called"); }

	virtual void reset() {};
	virtual bool runCycle() { return false; };
};



class LoopPullSourceSync :
	public PullSinkSync
{
private:
	PullSinkSync &m_target;
	const std::string m_target_port;

public:
	LoopPullSourceSync(
		const std::string &name,
		PullSinkSync &target,
		const std::string &target_port)
	:
		ManagerBlockSync(name),
		m_target(target),
		m_target_port(target_port)
	{
		registerPullInput(name);
	}

	virtual ~LoopPullSourceSync() {}

	PullSinkSync &getTarget() const
	{ return m_target; }

	const std::string &getTargetPort() const
	{ return m_target_port; }

	virtual void reset() {};
	virtual bool runCycle() { return false; };
};



class LoopPullSinkSync :
	public PullSourceSync
{
public:
	explicit LoopPullSinkSync(const std::string &name) :
		ManagerBlockSync(name)
	{
		registerPullOutput(name);
	}

	virtual ~LoopPullSinkSync() {}

	virtual void pullOutput(const t_port_number port)
	{ throw SimException("This method should never be called"); }

	virtual const Data &peekOutput(const t_port_number port) const
	{ throw SimException("This method should never be called"); }

	virtual bool isPullOutputEmpty(const t_port_number port) const
	{ throw SimException("This method should never be called"); }

	virtual bool isPullOutputDone(const t_port_number port) const
	{ throw SimException("This method should never be called"); }

	virtual void reset() {};
	virtual bool runCycle() { return false; };
};

} // namespace maxcompilersim

#endif /* LOOPS_H_ */
