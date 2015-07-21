#ifndef IFPGAREGS_H
#define IFPGAREGS_H

#include "ManagerSync.h"

namespace maxcompilersim {

class IFPGARegs : public ManagerBlockSync
{
public:
	IFPGARegs();

	virtual void reset() {}
	virtual bool runCycle() { return false; }
};

class SFARegs : public ManagerBlockSync
{
public:
	SFARegs();

	virtual void reset() {}
	virtual bool runCycle() { return false; }
};

} // namespace maxcompilersim

#endif // IFPGAREGS_H
