#ifndef FILEPULLSOURCE_H_
#define FILEPULLSOURCE_H_

#include <fstream>
#include <string>

#include "Data.h"
#include "ManagerInfrastructure.h"

namespace maxcompilersim {

class FilePullSource :
	public PullSourceSync
{
private:
	const std::string m_filename;
	std::ifstream m_file;

	Data m_current_word;
	bool m_eof;

public:
	FilePullSource(const std::string &name, const std::string &filename, uint bitwidth);

	virtual ~FilePullSource() {}

	virtual void pullOutput(const t_port_number);
	virtual const Data &peekOutput(const t_port_number) const;

	virtual bool isPullOutputEmpty(const t_port_number) const;
	virtual bool isPullOutputDone(const t_port_number) const;

	virtual bool runCycle() { return false; }
	virtual void reset();

private:
	void readNextWord();
};

} // namespace maxcompilersim

#endif /* FILEPULLSOURCE_H_ */
