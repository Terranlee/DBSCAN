#ifndef PUSH_ASYNC_H_
#define PUSH_ASYNC_H_

#include <vector>

#include <boost/shared_ptr.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/mutex.hpp>

#include "AsyncHelpers.h"
#include "Data.h"
#include "SimException.h"

namespace maxcompilersim {

class PushSinkAsync {
	friend class PushSourceAsync; // for blockingPushInput
private:
	std::vector<MutexPtr>   m_input_mutexes;
	std::vector<CondVarPtr> m_input_stalled_conds;
	std::vector<CondVarPtr> m_input_waiting_conds;
	std::vector<bool>       m_input_stalled;

public:
	virtual ~PushSinkAsync() {}

protected:
	class InputLock {
		friend class PushSinkAsync;
	private:
		const size_t m_input_id;
		boost::shared_ptr<boost::unique_lock<boost::mutex> > m_lock_ptr;
		InputLock(const size_t input_id, boost::mutex &mutex) :
			m_input_id(input_id),
			m_lock_ptr(new boost::unique_lock<boost::mutex>(mutex))
		{}

	public:

		void unlock() {
			m_lock_ptr->unlock();
		}
	};

	explicit PushSinkAsync(const size_t num_inputs) :
		m_input_mutexes      (num_inputs),
		m_input_stalled_conds(num_inputs),
		m_input_waiting_conds(num_inputs),
		m_input_stalled      (num_inputs)
	{
		for (size_t i(0); i < num_inputs; ++i)
		{
			m_input_stalled_conds[i] = CondVarPtr(new CondVar());
			m_input_waiting_conds[i] = CondVarPtr(new CondVar());
			m_input_mutexes[i] = MutexPtr(new Mutex());
		}
	}

	InputLock getInputLock(const size_t input_id) {
		assertValidInputID(input_id);
		return InputLock(input_id, *m_input_mutexes[input_id]);
	}

	void setInputStalled(InputLock &lock, const bool stalled) {
		if(!stalled && m_input_stalled[lock.m_input_id])
			m_input_stalled_conds[lock.m_input_id]->notify_one();
		m_input_stalled[lock.m_input_id] = stalled;
	}

	void waitInput(InputLock &lock) {
		m_input_waiting_conds[lock.m_input_id]->wait(*lock.m_lock_ptr);
	}

	virtual void pushInput(InputLock &lock, size_t input_id,
							const Data &data) = 0;

private:
	void assertValidInputID(const size_t input_id) {
		if(input_id >= m_input_stalled.size())
			BOOST_THROW_EXCEPTION(
				SimException(bfmt("Invalid input ID: %1%") % input_id));
	}

	void blockingPushInput(const size_t input_id, Data data) {
		assertValidInputID(input_id);
		InputLock lock(getInputLock(input_id));
		while( m_input_stalled[input_id] )
			m_input_stalled_conds[input_id]->wait(*lock.m_lock_ptr);
		pushInput(lock, input_id, data);
		m_input_waiting_conds[input_id]->notify_one();
	}
};

class PushSourceAsync {
private:
	struct Connection {
		PushSinkAsync *m_dst;
		size_t m_dst_id;

		Connection() :
			m_dst(NULL)
		{}
	};

	std::vector<Connection> m_output_conn;

public:
	void connectOutput(const size_t output_id, PushSinkAsync &dst,
						const size_t dst_input_id) {
		if(output_id >= m_output_conn.size())
			BOOST_THROW_EXCEPTION(
				SimException(bfmt("Invalid output id: %1%") % output_id));

		Connection &conn(m_output_conn[output_id]);
		if(conn.m_dst)
			BOOST_THROW_EXCEPTION(
				SimException(
					bfmt("Cannot re-connect output: %1%") % output_id));

		conn.m_dst_id = dst_input_id;
		conn.m_dst = &dst;
	}

protected:
	explicit PushSourceAsync(const size_t num_outputs) :
		m_output_conn(num_outputs)
	{}

	virtual ~PushSourceAsync() {}

	void pushOutput(const size_t output_id, Data data) {
		assertValidOutputID(output_id);

		Connection &conn(m_output_conn[output_id]);
		conn.m_dst->blockingPushInput(output_id, data);
	}

private:
	void assertValidOutputID(const size_t output_id) {
		if(output_id >= m_output_conn.size())
			BOOST_THROW_EXCEPTION(
				SimException(bfmt("Invalid output id: %1%") % output_id));

		if(!m_output_conn[output_id].m_dst)
			BOOST_THROW_EXCEPTION(
				SimException(
					bfmt("Output not connected with ID%1%") % output_id));
	}
};

} // namespace maxcompilersim

#endif // PUSH_ASYNC_H_
