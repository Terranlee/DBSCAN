#include "stdsimheader.h"
//#include "DBSCANKernel.h"
//#include "HWTypes.h"
//#include "KernelManagerBlockSync.h"

using namespace maxcompilersim;

// Templated Types used in the kernel
template class HWOffsetFix<32,0,TWOSCOMPLEMENT>;
template class HWRawBits<1>;
template class HWFloat<8,24>;
template class HWOffsetFix<1,0,UNSIGNED>;
template class HWOffsetFix<48,0,UNSIGNED>;
template class HWOffsetFix<49,0,UNSIGNED>;
// add. templates from the default formatter 


// Templated Methods/Functions
template HWOffsetFix<1,0,UNSIGNED>and_fixed<>(const HWOffsetFix<1,0,UNSIGNED> &a, const HWOffsetFix<1,0,UNSIGNED> &b );
template HWFloat<8,24>add_float<>(const HWFloat<8,24> &a, const HWFloat<8,24> &b );
template HWOffsetFix<1,0,UNSIGNED> cast_bits2fixed<1,0,UNSIGNED>(const HWRawBits<1> &a);
template HWOffsetFix<1,0,UNSIGNED>gte_fixed<>(const HWOffsetFix<49,0,UNSIGNED> &a, const HWOffsetFix<49,0,UNSIGNED> &b );
template HWRawBits<1> cast_fixed2bits<>(const HWOffsetFix<1,0,UNSIGNED> &a);
template HWOffsetFix<1,0,UNSIGNED>eq_fixed<>(const HWOffsetFix<48,0,UNSIGNED> &a, const HWOffsetFix<48,0,UNSIGNED> &b );
template HWOffsetFix<1,0,UNSIGNED> KernelManagerBlockSync::getMappedRegValue< HWOffsetFix<1,0,UNSIGNED> >(const std::string &name);
template void KernelManagerBlockSync::writeOutput< HWFloat<8,24> >(const t_port_number port_number, const HWFloat<8,24> &value);
template HWOffsetFix<48,0,UNSIGNED> cast_fixed2fixed<48,0,UNSIGNED,TRUNCATE>(const HWOffsetFix<49,0,UNSIGNED> &a);
template HWFloat<8,24> KernelManagerBlockSync::getMappedRegValue< HWFloat<8,24> >(const std::string &name);
template HWOffsetFix<48,0,UNSIGNED> KernelManagerBlockSync::getMappedRegValue< HWOffsetFix<48,0,UNSIGNED> >(const std::string &name);
template HWOffsetFix<49,0,UNSIGNED>add_fixed<49,0,UNSIGNED,TRUNCATE>(const HWOffsetFix<49,0,UNSIGNED> &a, const HWOffsetFix<49,0,UNSIGNED> &b );
template HWOffsetFix<32,0,TWOSCOMPLEMENT> KernelManagerBlockSync::getMappedRegValue< HWOffsetFix<32,0,TWOSCOMPLEMENT> >(const std::string &name);
template HWOffsetFix<1,0,UNSIGNED>not_fixed<>(const HWOffsetFix<1,0,UNSIGNED> &a );
template HWFloat<8,24> KernelManagerBlockSync::readInput< HWFloat<8,24> >(const t_port_number port_number);
template void KernelManagerBlockSync::setMappedRegValue< HWOffsetFix<48,0,UNSIGNED> >(const std::string &name, const HWOffsetFix<48,0,UNSIGNED> & value);


// Additional Code

