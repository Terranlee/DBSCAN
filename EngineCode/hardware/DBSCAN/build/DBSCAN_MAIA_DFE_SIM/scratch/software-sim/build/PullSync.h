#ifndef PULL_SYNC_H_
#define PULL_SYNC_H_

#include <map>
#include <string>

#include "ManagerSync.h"
#include "Data.h"
#include "Connections.h"

// #define DO_PULL_ASSERT_VALID

namespace maxcompilersim {

class PullSourceSync; //forward
class PullSinkSync; //forward



/* PULLSYNC
 * Common base class for PullSync Interfaces.
 * Defines types needed in both, PullSinkSync and PullSourceSync.
 */
class PullSync {
protected:
	/* PULLCONNECTION
	 * Protected class implementing all low-level connection stuff for
	 * the pull interface. Notice that all blocks implementing a pull
	 * interface may access the connection's internals. If this needs
	 * to change, change PullConnection to a simple class definition.
	 * ( class PullConnection; )
	 */
	class PullConnection: public Connection {
	public:
		PullSourceSync& source;
		const t_port_number source_port;

		const PullSinkSync& sink;
		const t_port_number sink_port;

		bool last_pull;
		bool next_empty;
		bool next_done;
		PullConnection(PullSourceSync &src,
				t_port_number src_port,
				const PullSinkSync &snk,
				t_port_number snk_port);
		virtual PullConnection* clone(); // override

	};
	/* PULLCONNECTIONHANDLE
	 * Opaque class that wraps around a PULLCONNECTION
	 * Implements the common connection-interface for PULL connections.
	 * (See Connections.h)
	 */
	class PullConnectionHandle: public ConnectionHandle {
	public:
		PullConnectionHandle (PullConnection *connection):
			ConnectionHandle(connection)
		{}
	};

	typedef boost::shared_ptr<PullConnection> PullConnectionPtr;
	typedef std::vector<PullConnectionPtr> t_pull_connections;
	typedef std::map<t_port_number,std::string> t_pull_port_names;
};



/* PULLSINKSYNC
 * Target/Sink of a pull connection.
 * Defines the consumer-interface for a multi-port pull connection.
 */
class PullSinkSync: public PullSync, public virtual ManagerBlockSync {
	friend class ManagerDesignRunnerSync; // For updateConnections()

private:
	t_pull_connections m_pull_input_connections;
	t_pull_port_names  m_pull_input_port_names;

	bool updateConnections();
	void abortConnections();
	void resetConnections();

protected:
	// CONST
#ifdef DO_PULL_ASSERT_VALID
	void assertValidInputName(const std::string &input_name) const;
	void assertValidInputPort(t_port_number input_port) const;
#else
	void assertValidInputName(const std::string &input_name) const {};
	void assertValidInputPort(t_port_number input_port) const {};
#endif
	t_pull_connections::const_iterator begin() const;
	t_pull_connections::const_iterator end() const;

	// MODIFIERS
	PullSinkSync() :
		ManagerBlockSync("")
	{
	}

	virtual ~PullSinkSync() {}

	t_port_number registerPullInput(const std::string &input_name);
	const Data &pullInput(t_port_number input_port);

public:
	// CONST
	bool isPullInputEmpty(t_port_number input_port) const;
	bool isPullInputDone(t_port_number input_port) const;
	const std::string getPullInputName(t_port_number port_number) const;

	// MODIFIERS
	t_connection_handle connectPullInput(const std::string &input_name,
		PullSourceSync &src, const std::string &src_output_name);
	t_connection_handle connectPullInput(t_port_number input_port,
		PullSourceSync &src, t_port_number src_output_port);
};



/* PULLSOURCESYNC
 * Source of a pull connection.
 * Defines the producer-interface for a multi-port pull connection.
 */

class PullSourceSync: public PullSync, public virtual ManagerBlockSync {
	friend class PullSinkSync;         // for access to pull/peek
	friend class PullStreamStatusSync; // -"-

private:
	t_pull_connections m_pull_output_connections;
	t_pull_port_names  m_pull_output_port_names;

protected:
	// CONST
#ifdef DO_PULL_ASSERT_VALID
	void assertValidOutputName(const std::string &output_name) const;
	void assertValidOutputPort(t_port_number output_port) const;
#else
	void assertValidOutputName(const std::string &output_name) const {};
	void assertValidOutputPort(t_port_number output_port) const {};
#endif
	virtual const Data &peekOutput(t_port_number output_port) const = 0;

	// MODIFIERS
	PullSourceSync() :
		ManagerBlockSync("")
	{
	}
	virtual ~PullSourceSync() {}
	t_port_number registerPullOutput(const std::string &output_name);
	virtual void pullOutput(t_port_number output_port) = 0;

public:
	// CONST
	virtual bool isPullOutputEmpty(t_port_number output_port) const = 0;
	virtual bool isPullOutputDone(t_port_number output_port) const;
	const std::string getPullOutputName(t_port_number output_port) const;
};

} // namespace maxcompilersim

#endif // PULL_SYNC_H_
