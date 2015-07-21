#ifndef RUNLENGTHEXPANDER_H_
#define RUNLENGTHEXPANDER_H_

#include "ManagerSync.h"
#include "PushSync.h"
#include "Data.h"

#include <boost/smart_ptr/shared_ptr.hpp>


namespace maxcompilersim {

#define RUNLENGTHEXPANDER_BUFFER_SIZE 16

class RunLengthExpanderSync:
	public PushSinkSync,
	public PushSourceSync
{
private:
	const int m_input_width_bits;
	const int m_output_width_bits;

	t_port_number m_input_port;
	t_port_number m_output_port;

	Data m_input_buffer [RUNLENGTHEXPANDER_BUFFER_SIZE];
	int m_buffer_read;
	int m_buffer_write;
	bool m_start_output;
	int m_current_output_bit;

	std::list<Data> m_ready_output_buffer;
	Data m_output_buffer;

	void write(const bool value);
public:
	// CONST
	virtual bool isPushInputStalled(const t_port_number input_port) const;

	// MODIFIERS
	RunLengthExpanderSync(const std::string &name, const int input_width_bits, const int output_width_bits);
	virtual ~RunLengthExpanderSync() {}

	virtual void reset();
	virtual bool runCycle();
	virtual void pushInput(const t_port_number input_port, const Data &data);
};
typedef boost::shared_ptr<RunLengthExpanderSync> RunLengthExpanderSyncPtr;


} // namespace


#endif /* RUNLENGTHEXPANDER_H_ */
