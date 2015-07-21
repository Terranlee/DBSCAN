
#ifndef CONNECTIONS_H_
#define CONNECTIONS_H_

#include <stdint.h>

namespace maxcompilersim {

/*
 * Abstract connection class...
 */

class Connection {
public:
	virtual Connection* clone() = 0;
};

/*
 * Opaque ConnectionHandle class.
 * Really a pointer with some additional methods..
 * (Which are to be implemented *g*)
 */
class ConnectionHandle {
private:
	Connection * m_connection;

protected:
	ConnectionHandle(Connection* pc): m_connection(pc) {}

public:
	// just copy the pointer to the connection
	ConnectionHandle(const ConnectionHandle& other)
		: m_connection(other.m_connection) {}

	ConnectionHandle& operator=(ConnectionHandle other){
		std::swap( this->m_connection, other.m_connection );
		return *this;
	}

	~ConnectionHandle() {
		/* don't delete the connection
		   object, as it belongs to
		   the manager components      */
	}
};


/* Type of Connection-Handle  */
typedef ConnectionHandle t_connection_handle;

/* Type of port: t_port_number
 *                is defined as a 32-bit integer type.
 * Rationale: Ports are numbered from 0 to max so that we can easily
 *            use switch-statements.
 */
typedef uint32_t t_port_number;

} // namespace

#endif /* CONNECTIONS_H_ */
