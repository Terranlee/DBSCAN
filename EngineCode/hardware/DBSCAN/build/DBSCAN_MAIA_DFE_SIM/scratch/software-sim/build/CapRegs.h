#ifndef CAPREGS_H
#define CAPREGS_H

#include <string>

#include "ManagerSync.h"

namespace maxcompilersim {

class CapRegs : public ManagerBlockSync
{
public:
	CapRegs(
		const std::string &name,
		int checksum,
		int num_input_streams,
		int num_output_streams,
		int clk_mult,
		int clk_div,
		int flags,
		int app,
		int rev,
		int chain_length);

	virtual void reset() {}
	virtual bool runCycle() { return false; }
};

} // namespace maxcompilersim

#endif // CAPREGS_H
