#ifndef PULL_ASYNC_H_
#define PULL_ASYNC_H_

#include <vector>

#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/shared_ptr.hpp>

#include "AsyncHelpers.h"
#include "Data.h"
#include "SimException.h"

namespace maxcompilersim {

class PullSourceAsync {
	friend class PullSinkAsync; // for access to blockingPullOutput

private:
	std::vector<MutexPtr>   m_output_mutexes;
	std::vector<CondVarPtr> m_output_empty_conds;
	std::vector<CondVarPtr> m_output_waiting_conds;
	std::vector<bool>       m_output_empty;

protected:
	class OutputLock {
		friend class PullSourceAsync;
	private:
		const size_t m_output_id;
		boost::shared_ptr<boost::unique_lock<boost::mutex> > m_lock_ptr;
		OutputLock(const size_t output_id, boost::mutex &mutex) :
			m_output_id(output_id),
			m_lock_ptr(new boost::unique_lock<boost::mutex>(mutex))
		{}

	public:
		void unlock() {
			m_lock_ptr->unlock();
		}
	};

	explicit PullSourceAsync(const size_t num_outputs) :
		m_output_mutexes      (num_outputs),
		m_output_empty_conds  (num_outputs),
		m_output_waiting_conds(num_outputs),
		m_output_empty        (num_outputs)
	{
		for (size_t i(0); i < num_outputs; ++i)
		{
			m_output_mutexes[i] = MutexPtr(new Mutex());
			m_output_empty_conds[i] = CondVarPtr(new CondVar());
			m_output_waiting_conds[i] = CondVarPtr(new CondVar());
		}
	}

	virtual ~PullSourceAsync() {}

	OutputLock getOutputLock(const size_t output_id) {
		assertValidOutputID(output_id);
		return OutputLock(output_id, *m_output_mutexes[output_id]);
	}

	void setOutputEmpty(OutputLock &lock, const bool empty) {
		if(!empty && m_output_empty[lock.m_output_id])
			m_output_empty_conds[lock.m_output_id]->notify_one();
		m_output_empty[lock.m_output_id] = empty;
	}

	void waitOutput(OutputLock &lock) {
		m_output_waiting_conds[lock.m_output_id]->wait(*lock.m_lock_ptr);
	}

	virtual Data pullOutput(OutputLock &lock, size_t output_id) = 0;

private:
	Data blockingPullOutput(const size_t output_id) {
		assertValidOutputID(output_id);
		OutputLock lock(getOutputLock(output_id));
		while( m_output_empty[output_id] )
			m_output_empty_conds[output_id]->wait(*lock.m_lock_ptr);

		m_output_waiting_conds[output_id]->notify_one();

		return pullOutput(lock, output_id);
	}

	void assertValidOutputID(const size_t output_id) {
		if(output_id > m_output_empty.size())
			BOOST_THROW_EXCEPTION(
				SimException(
					bfmt("Output ID is out of range: %1%") % output_id));
	}
};

class PullSinkAsync {
private:
	struct Connection {
		PullSourceAsync *m_src;
		size_t m_src_id;

		Connection() :
			m_src(NULL)
		{}
	};

	std::vector<Connection> m_input_conn;

public:
	void connectInput(const size_t input_id, PullSourceAsync &src,
						const size_t src_output_id) {
		if(input_id >= m_input_conn.size())
			BOOST_THROW_EXCEPTION(
				SimException(bfmt("Invalid input id: %1%") % input_id));

		Connection &conn(m_input_conn[input_id]);
		if(conn.m_src)
			BOOST_THROW_EXCEPTION(
				SimException(bfmt("Cannot re-connect input: %1%") % input_id));

		conn.m_src_id = src_output_id;
		conn.m_src = &src;
	}

protected:
	explicit PullSinkAsync(const size_t num_inputs) :
		m_input_conn(num_inputs)
	{}

	Data pullInput(const size_t input_id) {
		assertValidInputID(input_id);
		Connection &conn(m_input_conn[input_id]);

		return conn.m_src->blockingPullOutput(conn.m_src_id);
	}

private:
	void assertValidInputID(const size_t input_id) {
		if(input_id >= m_input_conn.size())
			BOOST_THROW_EXCEPTION(
				SimException(bfmt("Invalid input id: %1%") % input_id));

		if(!m_input_conn[input_id].m_src)
			BOOST_THROW_EXCEPTION(
				SimException(
					bfmt("Input not connected with ID %1%") % input_id));
	}
};

} // namespace maxcompilersim

#endif // PULL_ASYNC_H_
