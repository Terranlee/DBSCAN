#ifndef PUSH_SYNC_H_
#define PUSH_SYNC_H_


#include <map>
#include <string>

#include "ManagerSync.h"
#include "Data.h"
#include "Connections.h"

// #define DO_PUSH_ASSERT_VALID

namespace maxcompilersim {

class PushSourceSync; // forward
class PushSinkSync; // forward


/* PUSHSYNC
 * Common base class for PushSync Interfaces.
 * Defines types needed in both, PushSinkSync and PushSourceSync.
 */
class PushSync {
protected:
	/* PUSHCONNECTION
	 * Protected class implementing all low-level connection stuff for
	 * the push interface. Notice that all blocks implementing a push
	 * interface may access the connection's internals. If this needs
	 * to change, change PushConnection to a simple class definition.
	 * ( class PushConnection; )
	 */
	class PushConnection: public Connection {
	public:
		const PushSourceSync& source;
		const t_port_number source_port;

		PushSinkSync& sink;
		const t_port_number sink_port;

		Data last_data;
		bool last_push;
		bool next_stalled;
		PushConnection(const PushSourceSync &src,
				t_port_number src_port,
				PushSinkSync &snk,
				t_port_number snk_port);
		virtual PushConnection* clone(); // override

	};
	/* PUSHCONNECTIONHANDLE
	 * Opaque class that wraps around a PUSHCONNECTION
	 * Implements the common connection-interface for PUSH connections.
	 */
	class PushConnectionHandle: public ConnectionHandle {
	public:
		PushConnectionHandle (PushConnection *connection):
			ConnectionHandle(connection)
		{}
	};

	typedef boost::shared_ptr<PushConnection> PushConnectionPtr;
	typedef std::vector<PushConnectionPtr> t_push_connections;
	typedef std::map<t_port_number,std::string> t_push_port_names;
};



/* PUSHSINKSYNC
 * Target/Sink of push connection.
 * Defines the consumer-interface for a multi-port push connection.
 */
class PushSinkSync: public PushSync, public virtual ManagerBlockSync {
	friend class PushSourceSync; // For access to m_push_input_connections

private:
	t_push_connections m_push_input_connections;
	t_push_port_names  m_push_input_port_names;

protected:
	// CONST
#ifdef DO_PUSH_ASSERT_VALID
	void assertValidInputName(const std::string &input_name) const; // avoid, slow
	void assertValidInputPort(t_port_number port_number) const;
#else
	void assertValidInputName(const std::string &input_name) const {};
	void assertValidInputPort(t_port_number port_number) const {};
#endif
	t_push_connections::const_iterator begin() const;
	t_push_connections::const_iterator end() const;

	// MODIFIERS
	PushSinkSync() :
		ManagerBlockSync("")
	{
	}
	t_port_number registerPushInput(const std::string &input_name = "");

public:
	// CONST
	virtual bool isPushInputStalled(t_port_number port_number) const = 0;
	bool isPushInputDone(t_port_number port_number) const;
	const std::string getPushInputName(t_port_number port_number) const;

	// MODIFIERS
	virtual ~PushSinkSync() {}
	virtual void pushInput(t_port_number port, const Data &data) = 0;
};



/* PUSHSOURCESYNC
 * Source of a push connection.
 * Defines the producer-interface for a multi-port push connection.
 */
class PushSourceSync: public PushSync, public virtual ManagerBlockSync {
	friend class ManagerDesignRunnerSync; // For updateConnections()

private:
	t_push_connections m_push_output_connections;
	t_push_port_names  m_push_output_port_names;

	bool updateConnections();
	void abortConnections();
	void resetConnections();

protected:
	// CONST
#ifdef DO_PUSH_ASSERT_VALID
	void assertValidOutputName(const std::string &output_name) const; // avoid, slow
	void assertValidOutputPort(t_port_number port_number) const;
#else
	void assertValidOutputName(const std::string &output_name) const {};
	void assertValidOutputPort(t_port_number port_number) const {};
#endif
	t_push_connections::const_iterator begin() const;
	t_push_connections::const_iterator end() const;

	// MODIFIERS
	PushSourceSync() :
		ManagerBlockSync("")
	{
	}
	t_port_number registerPushOutput(const std::string &output_name = "");
	void pushOutput(t_port_number port_number, const Data &data);

public:
	// CONST
	virtual bool isPushOutputDone(t_port_number port_number) const;
	bool isPushOutputStalled(t_port_number port_number) const;
	const std::string getPushOutputName(t_port_number port_number) const;

	// MODIFIERS
	virtual ~PushSourceSync() {}
	t_connection_handle connectPushOutput(const std::string &output_name,
		PushSinkSync &dst, const std::string &dst_input_id);
	t_connection_handle connectPushOutput(t_port_number local_port,
		PushSinkSync &dst, t_port_number remote_port);
};

} // namespace maxcompilersim

#endif // PUSH_SYNC_H_
