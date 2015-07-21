#ifndef DEBUGSTREAMS_H_
#define DEBUGSTREAMS_H_


#include <boost/shared_ptr.hpp>
#include <map>
#include <string>
#include <fstream>
#include <stdint.h>
#include "ManagerSync.h"


namespace maxcompilersim {

class DebugStreams {
private:
	// first is the name of destination stream
	// second is the message
	typedef std::pair<std::string, std::string> Message;

	typedef std::tr1::unordered_map<std::string, boost::shared_ptr<std::ostream> > StreamMap;
	typedef std::vector<Message> MessageList;
	typedef std::map<int, MessageList > MessageSeqMap;

	const ManagerBlockSync *const m_owner;

	StreamMap m_open_streams;
	MessageSeqMap m_messages;

	std::string m_block_name;
	std::string m_dump_dir;

public:
	explicit DebugStreams(const ManagerBlockSync *const block,  const std::string &block_name);

	void write(const std::string &stream_name, int seq, const std::string &message);
	void finishCycle();
	void reset();

	void setDumpDirectory(const std::string &dir_name);
	void flush();

private:
	std::ostream &getStream(const std::string &stream_name, std::ostream& default_stream);
	std::ostream &getDefaultDebugOStream() const;
	std::ostream &getDebugBackupOStream() const;

	static void nullDeallocatorForCout(std::ostream *) {}
};

} // maxcompilersim namespace

#endif /* DEBUGSTREAMS_H_ */
