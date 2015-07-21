#ifndef FDSTREAM_H_
#define FDSTREAM_H_

#include <sys/socket.h>

namespace maxcompilersim {

	/*
	 * Thin wrapper around a file descriptor that implements the std::ostream interface.
	 * This is straight from http://www.josuttis.com/cppcode/fdstream.html and served as
	 * a basis for boost's file_descriptor_sink. NOTE however, that the more advanced boost
	 * version does not work here for some obscure reason (most likely it's doing funny stuff
	 * to the file descriptor when it shouldn't).
	 *
	 * If we ever want to support non-Linux systems the portability of this class has to be
	 * reviewed.
	 */

	class fdoutbuf : public std::streambuf {
	protected:
		const int fd;

	public:
		fdoutbuf (int _fd) : fd(_fd) { }

	protected:
		virtual int_type overflow (int_type c) {
			if (c != EOF) {
				const char z = c;
				if (send(fd, &z, 1, MSG_NOSIGNAL) != 1)
					return EOF;
			}
			return c;
		}

		virtual std::streamsize xsputn (const char* s, std::streamsize num) {
			return send(fd, s, num, MSG_NOSIGNAL);
		}
	};

	class fdostream : public std::ostream {
	protected:
		fdoutbuf buf;

	public:
		fdostream (int fd) : std::ostream(0), buf(fd) {
			rdbuf(&buf);
		}
	};

} /* namespace maxcompilersim */

#endif /* FDSTREAM_H_ */
