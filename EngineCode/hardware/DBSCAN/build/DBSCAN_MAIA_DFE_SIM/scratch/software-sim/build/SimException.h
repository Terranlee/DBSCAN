#ifndef SIMEXCEPTION_H_
#define SIMEXCEPTION_H_

#include <exception>
#include <string>
#include <execinfo.h>

#include <boost/exception/error_info.hpp>
#include <boost/exception/exception.hpp>
#include <boost/format.hpp>

// This isn't used here but will be at throw sites ...
#include <boost/throw_exception.hpp>

namespace maxcompilersim {

typedef boost::format bfmt;

/* Base class for all exceptions used within the simulator.
 * Unless caught, the user will be notified about the exception
 * and will see the exception message!
 */
class SimException :
	public virtual boost::exception,
	public virtual std::exception
{
protected:
	std::string m_what;

	// to get useful stack traces, compile simulation
	// with debug information turned on AND
	// optimizations turned off...
	void doBackTrace()
	{
		void *array[11];
		size_t size = backtrace(array, 10);
		char **strings = backtrace_symbols (array, size);
		std::string backtrace;
		for (size_t i = 0; i < size; i++)
		{
			backtrace += "\n";
			backtrace += strings[i];
		}
		free (strings);
		m_what += "\n Backtrace: ";
		m_what += backtrace;
	}

public:
	SimException(const std::string &msg):
		m_what(msg)
	{
		doBackTrace();
	}

	SimException(const bfmt &fmt) :
		m_what(boost::str(fmt))
	{
		doBackTrace();
	}

	virtual ~SimException() throw () {}

	virtual const char *what() const throw() { return m_what.c_str(); }
};


/* Base class for all exceptions that need to stay silent:
 * The simulation will still terminate if this exception is not caught but
 * the user won't be notified. See SimTerminator.cpp for an example.
 * Hence, you really don't want to throw a SimCleanHaltException.
 */
class SimCleanHaltException :
	public SimException
{
public:
	SimCleanHaltException(const std::string &msg) :SimException(msg) {}

	SimCleanHaltException(const bfmt &fmt) : SimException(fmt) {}

	virtual ~SimCleanHaltException() throw () {}
};	

class InputException:
	public SimException
{
	std::string m_input_name;
public:
	InputException(const std::string &input_name, const std::string &msg) :SimException(msg), m_input_name(input_name) {}
	InputException(const std::string &input_name, const bfmt &fmt) : SimException(fmt), m_input_name(input_name) {}
	virtual ~InputException() throw () {}

	const std::string& getInputName() const throw() {return m_input_name; }
};

/* Thrown in JavaSim mode when input is done but
 * a kernel wants to read the input
 */
class InputDoneException:
	public InputException
{
public:
	InputDoneException(const std::string &input_name, const std::string &msg) :InputException(input_name,msg){}
	InputDoneException(const std::string &input_name, const bfmt &fmt) : InputException(input_name,fmt){}
	virtual ~InputDoneException() throw () {}
};

/* Thrown in JavaSim mode when a kernel attempts to
 * read from an empty input.
 */
class InputEmptyException:
	public InputException
{
public:
	InputEmptyException(const std::string &input_name, const std::string &msg) : InputException(input_name, msg) {}
	InputEmptyException(const std::string &input_name, const bfmt &fmt) : InputException(input_name, fmt) {}
	virtual ~InputEmptyException() throw () {}
};

}

#define SIMPLE_THROW(msg) \
	BOOST_THROW_EXCEPTION(SimException(msg))

#endif /* SIMEXCEPTION_H_ */
