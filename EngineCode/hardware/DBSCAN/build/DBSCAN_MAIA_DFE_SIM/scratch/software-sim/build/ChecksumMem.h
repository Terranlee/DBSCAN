#ifndef CHECKSUMMEM_H
#define CHECKSUMMEM_H

#include <string>

#include "ManagerSync.h"

namespace maxcompilersim {

class ChecksumMem : public ManagerBlockSync
{
public:
	ChecksumMem(
		const std::string &name,
		const std::string &sum);

	virtual void reset() {}
	virtual bool runCycle() { return false; }
};

} // namespace maxcompilersim

#endif // CHECKSUMMEM_H
