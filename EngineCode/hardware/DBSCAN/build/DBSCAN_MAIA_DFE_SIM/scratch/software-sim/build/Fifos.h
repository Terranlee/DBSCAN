#ifndef FIFOS_H_
#define FIFOS_H_

#include <string>

#include "Data.h"
#include "ManagerSync.h"
#include "PullSync.h"
#include "PushSync.h"

namespace maxcompilersim {

class Fifo {
	Data *const m_data;
	const size_t m_capacity;
	const size_t m_width;
	size_t m_head_pos;
	size_t m_tail_pos;
	size_t m_fill_size;

public:
	Fifo(size_t capacity, size_t width);
	~Fifo();

	void reset();
	bool isEmpty() const;
	bool isFull() const;
	void add(const Data &data);
	const Data &peek() const;
	Data remove();

	const size_t getCapacity() const {return m_capacity;}
	const size_t getFillSize() const {return m_fill_size;}
	const size_t getWidth() const {return m_width;}
};

class FifoSync :
	public virtual ManagerBlockSync
{
protected:
	Fifo m_fifo;

public:
	FifoSync() :
		ManagerBlockSync(""),
		m_fifo(0, 0)
	{
	}

	FifoSync(
		const std::string &name,
		bool has_flags,
		size_t capacity,
		size_t width);
};

class FifoPushToPushSync :
	public FifoSync,
	public PushSinkSync,
	public PushSourceSync
{
	bool m_done;
	const t_port_number m_input_port;
	const t_port_number m_output_port;
public:
	FifoPushToPushSync(
		const std::string &name,
		bool has_flags,
		size_t capacity,
		size_t width = 32);

	virtual void reset();

	/*
	 * PushSink
	 */

	virtual void pushInput(const t_port_number input_port, const Data &data);

	virtual bool isPushInputStalled(const t_port_number input_port) const;
	virtual bool isPushOutputDone(const t_port_number input_port) const;

	/*
	 * PushSource
	 */

	virtual bool runCycle();
};

class FifoPullToPullSync :
	public FifoSync,
	public PullSinkSync,
	public PullSourceSync
{
	bool m_done;
	const t_port_number m_input_port;
	const t_port_number m_output_port;

public:
	FifoPullToPullSync(
		const std::string &name,
		bool has_flags,
		size_t capacity,
		size_t width);

	virtual void reset();

	/*
	 * PullSource
	 */

	virtual void pullOutput(const t_port_number port_number);
	virtual const Data &peekOutput(const t_port_number port_number) const;
	virtual bool isPullOutputEmpty(const t_port_number port_number) const;
	virtual bool isPullOutputDone(const t_port_number port_number) const;

	/*
	 * PullSink
	 */

	virtual bool runCycle();
};

class FifoPushToPullSync :
	public FifoSync,
	public PushSinkSync,
	public PullSourceSync
{
private:
	bool m_is_done;
	const t_port_number m_input_port;
	const t_port_number m_output_port;

public:
	FifoPushToPullSync(
		const std::string &name,
		bool has_flags,
		size_t capacity,
		size_t width = 32);

	virtual void reset();


	/*
	 * PushSink
	 */

	virtual void pushInput(const t_port_number input_port, const Data &data);
	virtual bool isPushInputStalled(const t_port_number input_port) const;


	/*
	 * PullSource
	 */

	virtual void pullOutput(const t_port_number port_number);
	virtual const Data &peekOutput(const t_port_number port_number) const;
	virtual bool isPullOutputEmpty(const t_port_number port_number) const;
	virtual bool isPullOutputDone(const t_port_number port_number) const;


	virtual bool runCycle();
};

class FifoPullToPushSync :
	public FifoSync,
	public PullSinkSync,
	public PushSourceSync
{
	const t_port_number m_input_port;
	const t_port_number m_output_port;

public:
	FifoPullToPushSync(const std::string &name,
			bool has_flags,
			size_t capacity,
			size_t width = 32);

	virtual void reset();

	/*
	 * Entirely active operation
	 */

	virtual bool runCycle();
};

} // namespace maxcompilersim

#endif // FIFOS_H_
