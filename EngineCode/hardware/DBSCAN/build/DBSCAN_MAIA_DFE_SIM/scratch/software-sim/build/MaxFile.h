#ifndef MAX_FILE_H_
#define MAX_FILE_H_

#define _QUOTE(what) #what
#define QUOTE(what) _QUOTE(what)

#include <tr1/unordered_map>
#include <string>
#include <boost/scoped_ptr.hpp>
#include <boost/make_shared.hpp>

//#define TRACE_MAX_FILE
#ifdef TRACE_MAX_FILE
#include <iostream>
#endif

#include "CapRegs.h"
#include "IFPGARegs.h"
#include "ChecksumMem.h"
#include "Demux.h"
#include "DualAspectMux.h"
#include "DualAspectReg.h"
#include "Fanout.h"
#include "Fifos.h"
#include "Loops.h"
#include "KernelManagerBlockSync.h"
#include "Mux.h"
#include "PCIe.h"
#include "PullPushAdapter.h"
#include "FilePullSource.h"
#include "FilePushSink.h"
#include "Watch.h"
#include "AddressGenerator.h"
#include "AddressGeneratorStrings.h"
#include "MemoryControllerPro.h"
#include "MemoryControllerStrings.h"
#include "MemoryControllerRAM.h"
#include "MemoryControllerCommand.h"
#include "MemoryControllerStreams.h"
#include "MemoryControllerConfig.h"
#include "StreamStatus.h"
#include "StructToGroupDummy.h"
#include "RunLengthExpander.h"
#include "TCP.h"
#include "Ethernet.h"
#include "Timestamp.h"


#define INCLUDE_GENERATED_CPP_HEADERS
#include QUOTE(MAXFILE_INC)
#undef INCLUDE_GENERATED_CPP_HEADERS

namespace maxcompilersim {

#define MANAGER_BLOCK_FIELD(name) \
	m_node_ ## name

class MaxFile {
	// Note these are pointers as in the past kernel objects have
	// been too big to fit on the stack!
	#define MANAGER_NODE_CPP_SIM_MODEL_CTOR(name, type, args...) \
		boost::scoped_ptr< type > MANAGER_BLOCK_FIELD(name);
	#include QUOTE(MAXFILE_INC)
	#undef MANAGER_NODE_CPP_SIM_MODEL_CTOR

	ManagerDesignSync m_design;
	std::tr1::unordered_map<std::string, ManagerBlock*> m_block_map;
	std::tr1::unordered_map<std::string, int> m_params_map;
	std::tr1::unordered_map<std::string, std::string> m_capabilities_map;
	const bool m_dummy;

public:
	MaxFile(const std::string &simulated_system_name) :
		#define MANAGER_NODE_CPP_SIM_MODEL_CTOR(name, type, args...) \
			MANAGER_BLOCK_FIELD(name) (new type(args)),
		#include QUOTE(MAXFILE_INC)
		#undef MANAGER_NODE_CPP_SIM_MODEL_CTOR

		m_dummy(false)
	{
		// This is only used by the Ethernet block for now so
		// avoid warning if one is not present.
		(void)simulated_system_name;

		#define MANAGER_NODE_CPP_SIM_MODEL_CTOR(name, type, args...) \
			m_design.addBlock(*MANAGER_BLOCK_FIELD(name)); \
			m_block_map[#name] = MANAGER_BLOCK_FIELD(name).get();

		#define MANAGER_NODE_CPP_SIM_MODEL_SETUP(name, function, args...) \
			MANAGER_BLOCK_FIELD(name)->function(args);

		#include QUOTE(MAXFILE_INC)
		#undef MANAGER_NODE_CPP_SIM_MODEL_SETUP
		#undef MANAGER_NODE_CPP_SIM_MODEL_CTOR



		#define MANAGER_STREAM(from, op, to, ip, args...) \
			connectStream(*MANAGER_BLOCK_FIELD(from), #op, *MANAGER_BLOCK_FIELD(to), #ip);

		#define REG(name, addr, args ...) \
			m_design.assignMappedRegisterAddress(addr, #name);

		#define MEM(name, address, type) \
			m_design.assignMappedMemoryAddress(address, #name);

		#define MEM2(name, address, type) \
			m_design.assignMappedMemoryAddress(address, #name);

		#define PARAM(key, value) \
			m_params_map[#key] = value;

		#define CAPABILITY(key, value) \
			m_capabilities_map[#key] = #value;

		#include QUOTE(MAXFILE_INC)

		#undef CAPABILITY
		#undef PARAM
		#undef MEM2
		#undef MEM
		#undef REG
		#undef MANAGER_STREAM
	}

	ManagerDesignSync &getManagerDesign() {
		return m_design;
	}

	template<class T>
	T *getManagerBlock(const std::string &name) {
		T* block = dynamic_cast<T*>(m_block_map[name]);
		if(block == NULL)
			BOOST_THROW_EXCEPTION(
				SimException(bfmt("Unknown block name: %1%") % name));

		return block;
	}

	int getParamValue(std::string name) {
		std::tr1::unordered_map<std::string, int>::iterator v = m_params_map.find(name);
		if(v == m_params_map.end())
			SIMPLE_THROW(bfmt("Param '%1%' not found in .max file.") % name);

#ifdef TRACE_MAX_FILE
		std::cout << "Get param " << name << " is " << (*v).second << std::endl;
#endif

		return (*v).second;
	}


	int getCapability(std::string name, std::string &result) {
		std::tr1::unordered_map<std::string,std::string>::iterator iter = m_capabilities_map.find(name);
		if (iter == m_capabilities_map.end()) {
#ifdef TRACE_MAX_FILE
		std::cout << "Get cap " << name << " failed. " << std::endl;
#endif
			return -1;
		} else {
#ifdef TRACE_MAX_FILE
		std::cout << "Get cap " << name << " is " << iter->second << std::endl;
#endif
			result = iter->second;
			return 0;
		}
	}

private:
	void connectStream(
		ManagerBlock &from,
		const std::string &op,
		ManagerBlock &to,
		const std::string &ip)
	{
		IOClass src_class = from.getIOClass(op);
		IOClass dst_class = to.getIOClass(ip);

		if(src_class != dst_class) {
			BOOST_THROW_EXCEPTION(SimException(
					bfmt("Output '%1%.%2%' is class %3% which is not compatible with '%4%.%5%' of class %6%.")
					% from.getName() % op % (src_class == PULL ? "PULL" : "PUSH")
					% to.getName()   % ip % (dst_class == PULL ? "PULL" : "PUSH")));

		} else if(src_class == PULL) {
			connectPullStream(
				dynamic_cast<PullSourceSync *>(&from),
				op,
				dynamic_cast<PullSinkSync *>(&to),
				ip);

		} else if(dst_class == PUSH) {
			connectPushStream(
				dynamic_cast<PushSourceSync *>(&from),
				op,
				dynamic_cast<PushSinkSync *>(&to),
				ip);

		} else {
			BOOST_THROW_EXCEPTION(SimException(bfmt("Unknown IO class.")));
		}
	}

	void connectPushStream(
		PushSourceSync *from,
		const std::string &op,
		PushSinkSync *to,
		const std::string &ip)
	{
		LoopPushSourceSync *to_loop_source = dynamic_cast<LoopPushSourceSync *>(to);
		LoopPushSinkSync   *from_loop_sink = dynamic_cast<LoopPushSinkSync *>(from);

		if(to_loop_source != NULL) {
			// bypass loop nodes
			from->connectPushOutput(op,
			                   to_loop_source->getTarget(),
			                   to_loop_source->getTargetPort());

		} else if(from_loop_sink != NULL) {
			// ignore loops sinks, connection already made (above)
		} else {
			from->connectPushOutput(op, *to, ip);
		}
	}

	void connectPullStream(
		PullSourceSync *from,
		const std::string &op,
		PullSinkSync *to,
		const std::string &ip)
	{
		LoopPullSourceSync *to_loop_source = dynamic_cast<LoopPullSourceSync *>(to);
		LoopPullSinkSync   *from_loop_sink = dynamic_cast<LoopPullSinkSync *>(from);

		if(to_loop_source != NULL) {
			// bypass loop nodes
			to_loop_source->getTarget().connectPullInput(to_loop_source->getTargetPort(), *from, op);
		} else if(from_loop_sink != NULL) {
			// ignore loops sinks, connection already made (above)
		} else {
			to->connectPullInput(ip, *from, op);
		}
	}
};

#undef MANAGER_BLOCK_FIELD
#undef QUOTE
#undef _QUOTE

}; // namespace maxcompilersim

#endif // MAX_FILE_H_
