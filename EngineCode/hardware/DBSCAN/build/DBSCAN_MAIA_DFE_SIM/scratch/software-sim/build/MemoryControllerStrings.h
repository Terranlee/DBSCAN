#ifndef MEMORYCONTROLLERSTRINGS_H_
#define MEMORYCONTROLLERSTRINGS_H_
#include <boost/lexical_cast.hpp>


#define STRING_DEFAULT_MEMCTR "MemoryControllerPro"

// registers according to Memory Controller Pro Document
#define STRING_REG_INT_ENABLE_AND "Int_Enable_AND"
#define STRING_REG_INT_ENABLE_OR "Int_Disable_OR"
// debug regs
#define STRING_REG_NO_RDSTREAMS "reg_no_rdstreams"
#define STRING_REG_NO_WRSTREAMS "reg_no_wrstreams"
#define STRING_REG_PHY_INIT_DONE "phy_init_done"		// 1 = done
#define STRING_REG_MEMCTRL_IDLE "MemCtrl_Idle"			// 1 = idle
#define STRING_REG_MEMCMD_QEMPTY "MemCmd_QEmpty"		// 1 = empty
#define STRING_REG_MEMCMD_QFULL "MemCmd_QFull"					// 1 = full
#define STRING_REG_CMD_COUNTER(n) "CMD_Counter " + n
#define STRING_REG_DATA_COUNTER(n) "Data_Counter " + n
#define STRING_REG_ARB_CONTROL  "Arb_Control"

#define MEMORYCONTROLLER_REG_NAME(name) "MemoryControllerPro_" + std::string(name)

#define MEMORYCONTROLLER_CONTROL_STREAM_NAME(name) name
#define MEMORYCONTROLLER_INPUT_STREAM_NAME(grp_name,name) name
#define MEMORYCONTROLLER_OUTPUT_STREAM_NAME(grp_name,name) name

// internal stuff
#define INTERNAL_MEMORYCONTROLLER_CONTROL_STREAM_NAME(number) std::string("CTRL_") + boost::lexical_cast<std::string>(number)
#define INTERNAL_MEMORYCONTROLLER_INPUT_STREAM_NAME(grp_name,number) std::string("INPUT_") + grp_name + std::string("_") + boost::lexical_cast<std::string>(number)
#define INTERNAL_MEMORYCONTROLLER_OUTPUT_STREAM_NAME(grp_name,number) std::string("OUTPUT_") + grp_name + std::string("_") + boost::lexical_cast<std::string>(number)

#endif /* MEMORYCONTROLLERSTRINGS_H_ */
