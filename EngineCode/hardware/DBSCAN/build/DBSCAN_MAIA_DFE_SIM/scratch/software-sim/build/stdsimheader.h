
#ifndef STDSIMHEADER_H_
#define STDSIMHEADER_H_

/*
 *
 * This is the standard simulation header.
 * A precompiled version of this header is used 
 * during the compilation of simulation kernels.
 *
 * Include ALL necessary headers here..
 *
 */


/* NEVER define BOOST_NO_STD_LOCALE here,
 * as this will break the simulation: all the
 * kernel files are compiled without this #define.
 */
#include <boost/format.hpp>


#include "HWTypes.h"
#include "KernelManagerBlockSync.h"

#ifdef HAVE_KERNELS
#include "stdkernel_headers.h"
#endif


#ifdef OPTIMIZATION_LEVEL
	#define MIGHT_USE_PRECOMPILED_HEADER
#endif

#endif /* STDSIMHEADER_H_ */
