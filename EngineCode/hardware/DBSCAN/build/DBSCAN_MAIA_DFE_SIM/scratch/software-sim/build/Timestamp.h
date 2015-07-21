
#ifndef TIMESTAMP_H_
#define TIMESTAMP_H_

#include "ManagerSync.h"
#include "PullSync.h"

namespace maxcompilersim {


class TimestampSourceSync:
	public PullSourceSync
{

private:
	uint32_t m_precision;
	bool m_has_count;
	bool m_has_bcd;

	mutable Data m_currentWord;

protected:
	virtual const Data &peekOutput(const t_port_number output_port) const;

	virtual void pullOutput(const t_port_number output_port);

public:
	// CONST
	virtual bool isPullOutputEmpty(const t_port_number output_port) const;
	virtual bool isPullOutputDone(const t_port_number output_port) const;

	// MODIFIERS
	TimestampSourceSync (const std::string component_name,
			uint32_t precision, bool has_count, bool has_bcd);
	virtual ~TimestampSourceSync() throw() {}

	virtual void reset();
	virtual bool runCycle();
};


} // namespace

#endif /* TIMESTAMP_H_ */
