#ifndef DEBUGGABLE_H_
#define DEBUGGABLE_H_


#include <boost/shared_ptr.hpp>
#include <map>
#include <string>
#include <fstream>
#include <stdint.h>
#include "ManagerSync.h"
#include "DebugStreams.h"


namespace maxcompilersim {

	/*
	 * Interface inherited by classes that can write debug output.
	 * Currently, these are Kernels and ManagerStateMachines.
	 */

	class Debuggable {

	private:
		DebugStreams m_debug_streams;


	protected:
		explicit Debuggable(const ManagerBlockSync *const block,  const std::string &block_name) :
			m_debug_streams(block, block_name)
		{}

		DebugStreams* getDebugStreams() {
			return &m_debug_streams;
		}


	public:
		void setDumpDirectory(const std::string &dir_name) {
			m_debug_streams.setDumpDirectory(dir_name);
		}

		void flushDebugStreams() {
			m_debug_streams.flush();
		}

	};

} // maxcompilersim namespace

#endif /* DEBUGGABLE_H_ */
