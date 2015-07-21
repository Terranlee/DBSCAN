#ifndef ADDRESSGENERATORSTRINGS_H_
#define ADDRESSGENERATORSTRINGS_H_

//
// INPUT
//
// Names of input registers
#define STRING_REG_ADDR_EN "_Addr_En"
#define STRING_REG_CMD_SIZE "_CmdSize"
// Names of input registers for Linear 1D and Block 3D
#define STRING_REG_BLOCK_SIZE_X "_BlockSize_X"
#define STRING_REG_BLOCK_SIZE_Y "_BlockSize_Y"
#define STRING_REG_BLOCK_SIZE_Z "_BlockSize_Z"
#define STRING_REG_WARP_X "_Wrap_X"
#define STRING_REG_WARP_Y "_Wrap_Y"
#define STRING_REG_WARP_Z "_Wrap_Z"
#define STRING_REG_START_X_ADDR "_Start_X_Addr"
#define STRING_REG_START_Y_ADDR "_Start_Y_Addr"
#define STRING_REG_START_Z_ADDR "_Start_Z_Addr"
#define STRING_REG_STRIDE_Y "_Stride_Y"
#define STRING_REG_STRIDE_Z "_Stride_Z"
//Names of input registers for Stride 2D
#define STRING_REG_2D_START_ADDR "_Start_Addr"
#define STRING_REG_2D_SLOW_TOP "_Slow_Top"
#define STRING_REG_2D_SLOW_STRIDE_MODIFIED "_Slow_Stride_Modified"
#define STRING_REG_2D_SLOW_TOP_WRAP "_Slow_Top_Wrap"
#define STRING_REG_2D_SLOW_OFFSET_PLUS_START_ADDR "_Slow_Offset_plus_Start_Addr"
#define STRING_REG_2D_FAST_STRIDE "_Fast_Stride"
#define STRING_REG_2D_FAST_STRIDE_MUL "_Fast_Stride_MUL"
#define STRING_REG_2D_CMD_MUL_FASTSTRIDE "_CMD_Mul_FastStride"
#define STRING_REG_2D_FAST_TOP_CMDS "_Fast_Top_CMDs"
#define STRING_REG_2D_FAST_LASTCMD_SIZE "_Fast_LastCMD_Size"
#define STRING_REG_2D_CMD_SIZE "_Command_Size"
// offset regs in pentagraph mode
#define STRING_REG_OFFSET(a) std::string("_Offset_") + a
// complete reg name is assembled by the following macro
#define ADDRESSGENERATOR_INPUT_REG_NAME(cname,regname) /*name + "." + */ regname


//
// OUTPUT
//
// naming of output streams
#define ADDRESSGENERATOR_OUTPUT_STREAM_NAME(name,num) name + "_" + num


//
// DESCRIPTION (OUTPUT)
//
// description of output fields
#define STRING_OUT_ADDRESS "address"
#define STRING_OUT_ADDRESS_PAD "address_pad"
#define STRING_OUT_SIZE "size"
#define STRING_OUT_INC "inc"
#define STRING_OUT_INC_PAD "inc_pad"
#define STRING_OUT_STREAM_SEL "stream"
#define STRING_OUT_TAG "tag"


#endif /* ADDRESSGENERATORSTRINGS_H_ */
