//#define TRACE_MSI

#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <cstdlib>

#include <boost/exception/diagnostic_information.hpp>
#include <boost/foreach.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/filesystem.hpp>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "MaxFile.h"

#include "maxdriver.h"
#include "maxerrors.h"
#include "max_msi.h"

#ifndef _QUOTE
#define _QUOTE(what) #what
#endif

#ifndef QUOTE
#define QUOTE(what) _QUOTE(what)
#endif

using namespace maxcompilersim;
using namespace std;

struct max_msi_handle {
	typedef std::map<int, PCIeBlockSync *> InputStreamsMap;
	typedef std::map<int, PCIeBlockSync *> OutputStreamsMap;
	InputStreamsMap m_input_streams;
	OutputStreamsMap m_output_streams;

	MaxFile m_max_file;

	boost::scoped_ptr<ManagerDesignRunnerSync> m_runner;
	std::auto_ptr<std::string> m_error;
public:

	~max_msi_handle() {
		m_runner->shutdown();
		m_input_streams.clear();
		m_output_streams.clear();
	}

	max_msi_handle(const std::string &simulated_system_name, int device_index) :
		m_max_file(simulated_system_name)
	{
		#define PCIE_STREAM(name, dir, index) \
			if(string(#dir) == "STREAM_FROM_HOST") \
				m_input_streams[index] = \
					m_max_file.getManagerBlock<PCIeBlockSync>(#name); \
			else \
				m_output_streams[index] = \
					m_max_file.getManagerBlock<PCIeBlockSync>(#name);

		#include QUOTE(MAXFILE_INC)

		#undef PCIE_STREAM

		// create FIFOs for each PCIe block
		// FIXME: device_id
		typedef OutputStreamsMap::value_type OutputIttValue;
		BOOST_FOREACH(OutputIttValue p, m_output_streams)
			p.second->createFIFO(simulated_system_name, device_index, p.first, true);

		typedef InputStreamsMap::value_type InputIttValue;
		BOOST_FOREACH(InputIttValue p, m_input_streams)
			p.second->createFIFO(simulated_system_name, device_index, p.first, false);

		// Last of all, get the thing running ...
		m_runner.reset(new ManagerDesignRunnerSync(m_max_file.getManagerDesign()));
	}

	void reset() {
		m_runner->reset();
	}

	void setMappedElement(
		const int32_t addr,
		const uint64_t value,
		const int32_t mask)
	{
		m_runner->setMappedElement(addr, value, mask);
	}

	uint64_t getMappedElement(const int32_t addr) {
		return m_runner->getMappedElement(addr);
	}

	void mecWriteMappedRegisters(
		const uint32_t address,
		const mapped_registers_data_t *const data,
		const size_t item_count)
	{
		m_runner->mecWriteMappedRegisters(address, data, item_count);
	}

	void mecReadMappedRegisters(
		const uint32_t address,
		mapped_registers_data_t *const data,
		const size_t item_count)
	{
		m_runner->mecReadMappedRegisters(address, data, item_count);
	}

	void mecWriteMappedMemories(
		const uint32_t address,
		const mapped_memories_data_t *const data,
		const size_t item_count)
	{
		m_runner->mecWriteMappedMemories(address, data, item_count);
	}

	void mecReadMappedMemories(
		const uint32_t address,
		mapped_memories_data_t *const data,
		const size_t item_count)
	{
		m_runner->mecReadMappedMemories(address, data, item_count);
	}

	void streamAbort() {
		m_runner->stopThread();

		typedef OutputStreamsMap::value_type OutputIttValue;
		BOOST_FOREACH(OutputIttValue p, m_output_streams)
			p.second->abort();

		typedef InputStreamsMap::value_type InputIttValue;
		BOOST_FOREACH(InputIttValue p, m_input_streams)
			p.second->abort();

		m_runner->abortConnections();

		m_runner->startThread();
	}

	int waitForInterrupt(const void* connectionContext, int (*connectionDropped)(const void* connectionContext)) {
		return m_runner->waitForInterrupt(connectionContext, connectionDropped) ? 0 : -1;
	}

	void enableWatchnodes(const std::string &dir_name) {
		boost::filesystem::path dir_path(dir_name);
		if(!boost::filesystem::exists(dir_path)) {
			if(!boost::filesystem::create_directories(dir_path))
				std::cerr << "Failed to create watchnode directory '"
				<< dir_name << "'" << std::endl;
		}

		std::vector<WatchSync *> watch_blocks
			= m_max_file.getManagerDesign().getBlocksOfType<WatchSync>();

		BOOST_FOREACH(WatchSync *watch, watch_blocks)
			watch->enable(dir_name);

		//m_runner->setDumpDirectory(dir_name);  -- see enableFPrintf
	}

	void setSimParameter(const std::string &name, const std::string &value) {
		m_runner->setSimParameter(name, value);
	}

	const std::string getSimParameter(const std::string &name) {
		return m_runner->getSimParameter(name);
	}

	void enableFPrintf(const std::string &dir_name) {
		boost::filesystem::path dir_path(dir_name);
		if(!boost::filesystem::exists(dir_path)) {
			if(!boost::filesystem::create_directories(dir_path))
				std::cerr << "Failed to create fprintf directory '"
				<< dir_name << "'" << std::endl;
		}

		m_runner->setDumpDirectory(dir_name);
	}

	void disableWatchnodes() {
		std::vector<WatchSync *> watch_blocks
			= m_max_file.getManagerDesign().getBlocksOfType<WatchSync>();

		BOOST_FOREACH(WatchSync *watch, watch_blocks)
			watch->disable();

		m_runner->flushDebugStreams();
	}

	void flushWatchnodes() {
		std::vector<WatchSync *> watch_blocks
			= m_max_file.getManagerDesign().getBlocksOfType<WatchSync>();

		BOOST_FOREACH(WatchSync *watch, watch_blocks)
			watch->flush();

		m_runner->flushDebugStreams();
	}

	void restrictWatchnodes(const std::string &kernel_name, int begin, int end) {
		std::vector<WatchSync *> watch_blocks
			= m_max_file.getManagerDesign().getBlocksOfType<WatchSync>();

		BOOST_FOREACH(WatchSync *watch, watch_blocks)
			watch->restrict_(kernel_name, begin, end);
	}

	void redirectDebugOutput(const std::string &file_name) {
		m_runner->redirectDefaultDebugStream(file_name);
	}

	void redirectDebugOutput(int fd) {
		m_runner->redirectDefaultDebugStream(fd);
	}

	void enableDebugBackup(const std::string &file_name) {
		m_runner->redirectDebugBackup(file_name);
	}

	bool getHasError() {
		if (m_error.get())
			return true;
		m_error.reset(m_runner->tryGetAbortReason());

		return m_error.get() != 0;
	}

	void setError(const std::string &error) {
		m_error.reset(new std::string(error));
	}

	ssize_t drainExceptions(char **exceptions) {
		if (m_error.get() == 0) {
			*exceptions = NULL;
			return 0;
		}

		// use malloc as we are passing this string back to a C API
		// which will be responsible for deallocating it via free
		char *ex_str = static_cast<char *>(std::malloc(m_error->size()));
		if (ex_str == NULL)
			return -1;

		std::copy(m_error->begin(), m_error->end(), ex_str);
		ex_str[m_error->size()] = '\0';

		*exceptions = ex_str;

		return m_error->size();
	}
};

// MSI interface ...

#ifdef TRACE_MSI
#define TRACE_FUNC_ENTRY std::cout << "Entering " << __func__ << std::endl;
#define TRACE_FUNC_EXIT std::cout << "Exiting " << __func__ << std::endl;
#else
#define TRACE_FUNC_ENTRY
#define TRACE_FUNC_EXIT
#endif

#define CHECK_MSI_HANDLE(handle) \
	if (!(handle)) { \
		TRACE_FUNC_EXIT \
		return EMAX_SIM_NULL_HANDLE; \
	} \
	if ((handle)->getHasError()) { \
		TRACE_FUNC_EXIT \
		return EMAX_SIM_KERNEL_EXCEPTION; \
	}


int max_msi_initialize(max_msi_handle_t *const handle, const char *simulated_system_name, int device_index) try
{
	TRACE_FUNC_ENTRY

	if(!handle) {
		TRACE_FUNC_EXIT
		return EMAX_SIM_NULL_HANDLE;
	}

	std::string simulated_system_name_str(simulated_system_name);
	*handle = new max_msi_handle(simulated_system_name_str, device_index);

	TRACE_FUNC_EXIT
	return 0;
}
catch (boost::exception &e)
{
	std::cerr << "max_msi_initialize: " << boost::diagnostic_information(e);
	*handle = 0;
	return EMAX_SIM_EXCEPTION_DURING_INIT;
}
catch (...)
{
	std::cerr << "max_msi_initialize: " << boost::current_exception_diagnostic_information();
	*handle = 0;
	return EMAX_SIM_EXCEPTION_DURING_INIT;
}

// Crude but effective ...

#define CATCH_EXCEPTIONS \
catch (boost::exception &e) \
{ \
	handle->setError(boost::diagnostic_information(e)); \
	return EMAX_SIM_KERNEL_EXCEPTION; \
} \
catch (...) \
{ \
	handle->setError(boost::current_exception_diagnostic_information()); \
	return EMAX_SIM_KERNEL_EXCEPTION; \
}

int max_msi_get_capability(
	const max_msi_handle_t handle,
	const int32_t capability,
	int32_t *value)
try {
	TRACE_FUNC_ENTRY

	CHECK_MSI_HANDLE(handle);

	switch (capability) {
		case MAX_CAPABILITY_IN_STREAMS:
			*value = handle->m_input_streams.size();
			break;
		case MAX_CAPABILITY_OUT_STREAMS:
			*value = handle->m_output_streams.size();
			break;
		case MAX_CAPABILITY_CHAIN_LENGTH:
			*value = handle->m_max_file.getParamValue("CHAIN_LENGTH");
			break;
		case MAX_CAPABILITY_APP_ID:
			*value = handle->m_max_file.getParamValue("APP_ID");
			break;
		case MAX_CAPABILITY_REV_ID:
			*value = handle->m_max_file.getParamValue("REV_ID");
			break;
		default:
			TRACE_FUNC_EXIT
			return EMAXCAP_WRONG_ARG;
	}

	TRACE_FUNC_EXIT
	return 0;
}
CATCH_EXCEPTIONS

int32_t max_msi_enable_watchnodes(
	const max_msi_handle_t handle,
	const char *const dir_name)
try {
	TRACE_FUNC_ENTRY

	CHECK_MSI_HANDLE(handle);

	handle->enableWatchnodes(dir_name);

	TRACE_FUNC_EXIT
	return 0;
}
CATCH_EXCEPTIONS

int32_t max_msi_disable_watchnodes(const max_msi_handle_t handle) try
{
	TRACE_FUNC_ENTRY

	CHECK_MSI_HANDLE(handle);

	handle->disableWatchnodes();

	TRACE_FUNC_EXIT
	return 0;
}
CATCH_EXCEPTIONS

int32_t max_msi_flush_watchnodes(const max_msi_handle_t handle) try
{
	TRACE_FUNC_ENTRY

	CHECK_MSI_HANDLE(handle);

	handle->flushWatchnodes();

	TRACE_FUNC_EXIT
	return 0;
}
CATCH_EXCEPTIONS

int32_t max_msi_restrict_watchnodes(
	const max_msi_handle_t handle,
	const char *const kernel_name,
	int begin,
	int end)
try {
	TRACE_FUNC_ENTRY

	CHECK_MSI_HANDLE(handle);

	handle->restrictWatchnodes(kernel_name, begin, end);

	TRACE_FUNC_EXIT
	return 0;
}
CATCH_EXCEPTIONS

int max_msi_reset(const max_msi_handle_t handle) try
{
	TRACE_FUNC_ENTRY

	CHECK_MSI_HANDLE(handle);

	handle->reset();

	TRACE_FUNC_EXIT
	return 0;
}
CATCH_EXCEPTIONS

int max_msi_destroy(const max_msi_handle_t handle) try
{
	TRACE_FUNC_ENTRY

	CHECK_MSI_HANDLE(handle);

	delete handle;

	TRACE_FUNC_EXIT
	return 0;
}
CATCH_EXCEPTIONS

int max_msi_set_mapped_element(
	const max_msi_handle_t handle,
	const int32_t addr,
	const uint32_t value,
	const int32_t mask)
try {
	TRACE_FUNC_ENTRY

	CHECK_MSI_HANDLE(handle);

	handle->setMappedElement(addr, value, mask);

	TRACE_FUNC_EXIT
	return 0;
}
CATCH_EXCEPTIONS

int max_msi_get_mapped_element(
	const max_msi_handle_t handle,
	const int32_t addr,
	uint32_t *const value)
try {
	TRACE_FUNC_ENTRY

	CHECK_MSI_HANDLE(handle);

	*value = handle->getMappedElement(addr);

	TRACE_FUNC_EXIT
	return 0;
}
CATCH_EXCEPTIONS

int max_msi_mec_write_mapped_registers(
	const max_msi_handle_t handle,
	const uint32_t address,
	const mapped_registers_data_t *const data,
	const size_t item_count)
try {
	TRACE_FUNC_ENTRY

	CHECK_MSI_HANDLE(handle);

	handle->mecWriteMappedRegisters(LOGICAL_REGISTERS_ADDRESS_OFFSET + address,
									data, item_count);

	TRACE_FUNC_EXIT
	return 0;
}
CATCH_EXCEPTIONS

int max_msi_mec_read_mapped_registers(
	const max_msi_handle_t handle,
	const uint32_t address,
	mapped_registers_data_t *const data,
	const size_t item_count)
try {
	TRACE_FUNC_ENTRY

	CHECK_MSI_HANDLE(handle);

	handle->mecReadMappedRegisters(LOGICAL_REGISTERS_ADDRESS_OFFSET + address,
									data, item_count);

	TRACE_FUNC_EXIT
	return 0;
}
CATCH_EXCEPTIONS

int max_msi_mec_write_mapped_memories(
	const max_msi_handle_t handle,
	const uint32_t address,
	const mapped_memories_data_t *const data,
	const size_t item_count)
try {
	TRACE_FUNC_ENTRY

	CHECK_MSI_HANDLE(handle);

	handle->mecWriteMappedMemories(address, data, item_count);

	TRACE_FUNC_EXIT
	return 0;
}
CATCH_EXCEPTIONS

int max_msi_mec_read_mapped_memories(
	const max_msi_handle_t handle,
	const uint32_t address,
	mapped_memories_data_t *const data,
	const size_t item_count)
try {
	TRACE_FUNC_ENTRY

	CHECK_MSI_HANDLE(handle);

	handle->mecReadMappedMemories(address, data, item_count);

	TRACE_FUNC_EXIT
	return 0;
}
CATCH_EXCEPTIONS

int max_msi_stream_abort(const max_msi_handle_t handle) try
{
	TRACE_FUNC_ENTRY

	CHECK_MSI_HANDLE(handle);

	handle->streamAbort();

	TRACE_FUNC_EXIT
	return 0;
}
CATCH_EXCEPTIONS

int max_msi_wait_stream_irq_cb(const max_msi_handle_t handle, int, const void* connectionContext, int (*connectionDropped)(const void* connectionContext)) try
{
	TRACE_FUNC_ENTRY

	CHECK_MSI_HANDLE(handle);

	int err = handle->waitForInterrupt(connectionContext, connectionDropped);

	TRACE_FUNC_EXIT
	return err;
}
CATCH_EXCEPTIONS

int max_msi_wait_stream_irq(const max_msi_handle_t handle, int)
{
	std::cout << "Warning: Bitstream uses deprecated max_msi_wait_stream_irq function!" << std::endl;
	return max_msi_wait_stream_irq_cb(handle,0,NULL,NULL);
}

int max_msi_redirect_debug_output(const max_msi_handle_t handle, const char *file_name) try
{
	TRACE_FUNC_ENTRY

	CHECK_MSI_HANDLE(handle);

	handle->redirectDebugOutput(file_name);

	TRACE_FUNC_EXIT
	return 0;
}
CATCH_EXCEPTIONS

int max_msi_redirect_debug_output_to_fd(const max_msi_handle_t handle, int fd) try
{
	TRACE_FUNC_ENTRY

	CHECK_MSI_HANDLE(handle);

	if(fd < 0) {
		TRACE_FUNC_EXIT
		return -1;
	}

	handle->redirectDebugOutput(fd);

	TRACE_FUNC_EXIT
	return 0;
}
CATCH_EXCEPTIONS

int max_msi_set_sim_parameter(
	const max_msi_handle_t handle,
	const char *name,
	const char *value)
try {
	TRACE_FUNC_ENTRY

	CHECK_MSI_HANDLE(handle);

	if(name == NULL) {
		TRACE_FUNC_EXIT;
		return -1;
	}

	if(value == NULL) {
		TRACE_FUNC_EXIT;
		return -1;
	}

	handle->setSimParameter(name, value);

	TRACE_FUNC_EXIT;
	return 0;
} CATCH_EXCEPTIONS

int max_msi_get_sim_parameter(
	const max_msi_handle_t handle,
	const char *name,
	char *buffer,
	size_t buffer_size)
try {
	TRACE_FUNC_ENTRY

	CHECK_MSI_HANDLE(handle);

	if(name == NULL) {
		TRACE_FUNC_EXIT;
		return -1;
	}

	if(buffer == NULL) {
		TRACE_FUNC_EXIT;
		return -1;
	}

	if(buffer_size == 0) {
		TRACE_FUNC_EXIT;
		return -1;
	}

	const std::string value = handle->getSimParameter(name);
	strncpy(buffer, value.c_str(), buffer_size);
	buffer[buffer_size - 1] = '\0'; // strncpy may omits the trailing 0 if there's not enough space

	TRACE_FUNC_EXIT;
	return 0;
} CATCH_EXCEPTIONS

int max_msi_enable_fprintf(
	const max_msi_handle_t handle,
	const char *const dir_name)
try {
	TRACE_FUNC_ENTRY

	CHECK_MSI_HANDLE(handle);

	handle->enableFPrintf(dir_name);

	TRACE_FUNC_EXIT
	return 0;
}
CATCH_EXCEPTIONS

int max_msi_enable_debug_backup(
	const max_msi_handle_t handle,
	const char *const file_name)
try {
	TRACE_FUNC_ENTRY

	CHECK_MSI_HANDLE(handle);

	handle->enableDebugBackup(file_name);

	TRACE_FUNC_EXIT
	return 0;
}
CATCH_EXCEPTIONS

int max_msi_drain_exceptions(
	max_msi_handle_t handle,
	size_t *str_len,
	char **str
)
try {
	TRACE_FUNC_ENTRY

	if (!handle) {
		TRACE_FUNC_EXIT
		return EMAX_SIM_NULL_HANDLE;
	}

	const ssize_t total_str_len = handle->drainExceptions(str);
	if (total_str_len == -1) {
		*str_len = 0;
		*str = NULL;

		// currently, the only reason for drainExceptions to fail should
		// be memory allocation
		TRACE_FUNC_EXIT
		return EMAXMALLOCFAIL;
	}

	*str_len = total_str_len;

	TRACE_FUNC_EXIT
	return 0;
}
CATCH_EXCEPTIONS
