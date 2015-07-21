#ifndef HWTYPES_H_
#define HWTYPES_H_

#include <stdint.h>

namespace maxcompilersim {

//****************************************************************************************************************************************************
// Utilities                                                                                                                                         *
//****************************************************************************************************************************************************

// uint shortcut
typedef unsigned int uint;

// Convert doubles to bits
union DoubleBits {
	double d;
	uint64_t b;
};

// Compile time error checking
//template<bool> struct CompileTimeError;
//template<> struct CompileTimeError<true> {};
//#define COMPILER_CHECK(expr, msg) { CompileTimeError<(expr) != 0> ERROR_##msg; (void)ERROR_##msg; }

// Useful for compile-time method selection. See A. Alexandrescu: Modern C++ Design, Section 2.4, pp.29-31
template <int v>
struct Int2Type { enum { value = v }; };

// Template function to determine the minimum number of bits required to hold the positive number num
template <int num, int bitsize> struct _MinBitSize    { enum { value = _MinBitSize<(num >> 1), bitsize + 1>::value }; };
template <int bitsize> struct _MinBitSize<0, bitsize> { enum { value = bitsize }; };
template <int num>
struct MinBitSize {
	enum { value = _MinBitSize<num, 0>::value };
};


//****************************************************************************************************************************************************
// Enums                                                                                                                                             *
//****************************************************************************************************************************************************

enum SignMode	{ UNSIGNED, TWOSCOMPLEMENT };		// for HWOffsetFix
enum RoundMode	{ TRUNCATE, TONEAR, TONEAREVEN }; 	// for operations on HWOffsetFix (HWFloat always rounds TONEAREVEN)
enum _CompareOp	{ EQ, NEQ, GT, GTE, LT, LTE };		// for internal use, e.g. during fix-point comparison


//****************************************************************************************************************************************************
// Class prototypes                                                                                                                                  *
//****************************************************************************************************************************************************

#define CLASS_varint_u \
	template <uint size> class varint_u
CLASS_varint_u;

#define CLASS_varint_s \
	template <uint size> class varint_s
CLASS_varint_s;

#define CLASS_HWRawBits \
	template <uint size> class HWRawBits
CLASS_HWRawBits;

#define CLASS_HWOffsetFix \
	template <uint size, int offset, SignMode sign_mode> class HWOffsetFix
CLASS_HWOffsetFix;

#define CLASS_HWFloat \
	template <uint esize, uint msize> class HWFloat
CLASS_HWFloat;


//****************************************************************************************************************************************************
// Various shortcuts                                                                                                                                 *
//****************************************************************************************************************************************************

typedef HWOffsetFix<1,0,UNSIGNED> ExceptOverflow;
typedef HWOffsetFix<4,0,UNSIGNED> ExceptFloat;

#define HWEXCEPT		, bool hw_exception
#define EXCEPTOVERFLOW	ExceptOverflow *exception
#define EXCEPTFLOAT		ExceptFloat *exception

typedef HWOffsetFix<1,0,UNSIGNED> HWBool;

#define CONST_true(DEF)															\
	DEF	HWBool const_true()
CONST_true();

#define CONST_false(DEF)														\
	DEF HWBool const_false()
CONST_false();

#define CONST_bool(DEF)															\
	DEF HWBool const_bool(bool v)
CONST_bool();


//****************************************************************************************************************************************************
// Casting method prototypes                                                                                                                         *
//****************************************************************************************************************************************************

#define CAST_float2bits(DEF)													\
	template <																	\
		uint a_esize, uint a_msize												\
	> DEF																		\
	HWRawBits<a_esize + a_msize>												\
	cast_float2bits(const HWFloat<a_esize, a_msize> &a)
CAST_float2bits();

#define CAST_bits2float(DEF)													\
	template <																	\
		uint r_esize, uint r_msize												\
	> DEF																		\
	HWFloat<r_esize, r_msize>													\
	cast_bits2float(const HWRawBits<r_esize+r_msize> &a)
CAST_bits2float();

#define CAST_fixed2bits(DEF)													\
	template <																	\
		uint a_size, int a_offset, SignMode a_sign_mode							\
	> DEF																		\
	HWRawBits<a_size>															\
	cast_fixed2bits(const HWOffsetFix<a_size, a_offset, a_sign_mode> &a)
CAST_fixed2bits();

#define CAST_bits2fixed(DEF)													\
	template <																	\
		uint r_size, int r_offset, SignMode r_sign_mode							\
	> DEF																		\
	HWOffsetFix<r_size, r_offset, r_sign_mode>									\
	cast_bits2fixed(const HWRawBits<r_size> &a)
CAST_bits2fixed();

#define CAST_bits2bits(DEF)														\
	template <																	\
		uint r_size, 															\
		uint a_size																\
	> DEF																		\
	HWRawBits<r_size>															\
	cast_bits2bits(const HWRawBits<a_size> &a)
CAST_bits2bits();

#define CAST_float2float(DEF, TEMPLATE, EXCEPT...)								\
	template <																	\
		uint r_esize, uint r_msize, 											\
		uint a_esize, uint a_msize												\
		TEMPLATE																\
	> DEF																		\
	HWFloat<r_esize, r_msize>													\
	cast_float2float(const HWFloat<a_esize, a_msize> &a ,##EXCEPT)
CAST_float2float(,HWEXCEPT,EXCEPTFLOAT);

#define CAST_fixed2fixed(DEF, TEMPLATE, EXCEPT...)								\
	template <																	\
		uint r_size, int r_offset, SignMode r_sign_mode,						\
		RoundMode round_mode,													\
		uint a_size, int a_offset, SignMode a_sign_mode							\
		TEMPLATE																\
	> DEF																		\
	HWOffsetFix<r_size, r_offset, r_sign_mode>									\
	cast_fixed2fixed(const HWOffsetFix<a_size, a_offset, a_sign_mode> &a ,##EXCEPT)
CAST_fixed2fixed(,HWEXCEPT,EXCEPTOVERFLOW);

#define CAST_float2fixed(DEF, TEMPLATE, EXCEPT...)								\
	template <																	\
		uint r_size, int r_offset, SignMode r_sign_mode,						\
		uint a_esize, uint a_msize												\
		TEMPLATE																\
	> DEF																		\
	HWOffsetFix<r_size, r_offset, r_sign_mode>									\
	cast_float2fixed(const HWFloat<a_esize, a_msize> &a ,##EXCEPT)
CAST_float2fixed(,HWEXCEPT,EXCEPTFLOAT);

#define CAST_fixed2float(DEF, TEMPLATE, EXCEPT...)								\
	template <																	\
		uint r_esize, uint r_msize, 											\
		uint a_size, int a_offset, SignMode a_sign_mode							\
		TEMPLATE																\
	> DEF																		\
	HWFloat<r_esize, r_msize>													\
	cast_fixed2float(const HWOffsetFix<a_size, a_offset, a_sign_mode> &a ,##EXCEPT)
CAST_fixed2float(,HWEXCEPT,EXCEPTFLOAT);


//****************************************************************************************************************************************************
// Floating point arithmetic                                                                                                                         *
//****************************************************************************************************************************************************

#define ADD_float(DEF, TEMPLATE, EXCEPT...)										\
	template <																	\
		uint r_esize, uint r_msize												\
		TEMPLATE																\
	> DEF																		\
	HWFloat<r_esize, r_msize>													\
	add_float(const HWFloat<r_esize, r_msize> &a, const HWFloat<r_esize, r_msize> &b ,##EXCEPT)
ADD_float(,HWEXCEPT,EXCEPTFLOAT);

#define SUB_float(DEF, TEMPLATE, EXCEPT...)										\
	template <																	\
		uint r_esize, uint r_msize												\
		TEMPLATE																\
	> DEF																		\
	HWFloat<r_esize, r_msize>													\
	sub_float(const HWFloat<r_esize, r_msize> &a, const HWFloat<r_esize, r_msize> &b ,##EXCEPT)
SUB_float(,HWEXCEPT,EXCEPTFLOAT);

#define MUL_float(DEF, TEMPLATE, EXCEPT...)										\
	template <																	\
		uint r_esize, uint r_msize												\
		TEMPLATE																\
	> DEF																		\
	HWFloat<r_esize, r_msize>													\
	mul_float(const HWFloat<r_esize, r_msize> &a, const HWFloat<r_esize, r_msize> &b ,##EXCEPT)
MUL_float(,HWEXCEPT,EXCEPTFLOAT);

#define DIV_float(DEF, TEMPLATE, EXCEPT...)										\
	template <																	\
		uint r_esize, uint r_msize												\
		TEMPLATE																\
	> DEF																		\
	HWFloat<r_esize, r_msize>													\
	div_float(const HWFloat<r_esize, r_msize> &a, const HWFloat<r_esize, r_msize> &b ,##EXCEPT)
DIV_float(,HWEXCEPT,EXCEPTFLOAT);

#define NEG_float(DEF)															\
	template <																	\
		uint r_esize, uint r_msize												\
	> DEF																		\
	HWFloat<r_esize, r_msize>													\
	neg_float(const HWFloat<r_esize, r_msize> &a)
NEG_float();

#define SQRT_float(DEF, TEMPLATE, EXCEPT...)									\
	template <																	\
		uint r_esize, uint r_msize												\
		TEMPLATE																\
	> DEF																		\
	HWFloat<r_esize, r_msize>													\
	sqrt_float(const HWFloat<r_esize, r_msize> &a ,##EXCEPT)
SQRT_float(,HWEXCEPT,EXCEPTFLOAT);


//****************************************************************************************************************************************************
// Floating point comparison                                                                                                                         *
//****************************************************************************************************************************************************

#define EQ_float(DEF)															\
	template <																	\
		uint a_esize, uint a_msize												\
	> DEF																		\
	HWBool																		\
	eq_float(const HWFloat<a_esize, a_msize> &a, const HWFloat<a_esize, a_msize> &b)
EQ_float();

#define NEQ_float(DEF)															\
	template <																	\
		uint a_esize, uint a_msize												\
	> DEF																		\
	HWBool																		\
	neq_float(const HWFloat<a_esize, a_msize> &a, const HWFloat<a_esize, a_msize> &b)
NEQ_float();

#define GT_float(DEF)															\
	template <																	\
		uint a_esize, uint a_msize												\
	> DEF																		\
	HWBool																		\
	gt_float(const HWFloat<a_esize, a_msize> &a, const HWFloat<a_esize, a_msize> &b)
GT_float();

#define GTE_float(DEF)															\
	template <																	\
		uint a_esize, uint a_msize												\
	> DEF																		\
	HWBool																		\
	gte_float(const HWFloat<a_esize, a_msize> &a, const HWFloat<a_esize, a_msize> &b)
GTE_float();

#define LT_float(DEF)															\
	template <																	\
		uint a_esize, uint a_msize												\
	> DEF																		\
	HWBool																		\
	lt_float(const HWFloat<a_esize, a_msize> &a, const HWFloat<a_esize, a_msize> &b)
LT_float();

#define LTE_float(DEF)															\
	template <																	\
		uint a_esize, uint a_msize												\
	> DEF																		\
	HWBool																		\
	lte_float(const HWFloat<a_esize, a_msize> &a, const HWFloat<a_esize, a_msize> &b)
LTE_float();



//****************************************************************************************************************************************************
// Fixed point arithmetic                                                                                                                            *
//****************************************************************************************************************************************************

#define ADD_fixed(DEF, TEMPLATE, EXCEPT...)										\
	template <																	\
		uint r_size, int r_offset, SignMode r_sign_mode,						\
		RoundMode round_mode,													\
		uint a_size, int a_offset, SignMode a_sign_mode,						\
		uint b_size, int b_offset, SignMode b_sign_mode							\
		TEMPLATE																\
	> DEF																		\
	HWOffsetFix<r_size, r_offset, r_sign_mode>									\
	add_fixed(	const HWOffsetFix<a_size, a_offset, a_sign_mode> &a,			\
				const HWOffsetFix<b_size, b_offset, b_sign_mode> &b ,##EXCEPT )
ADD_fixed(,HWEXCEPT,EXCEPTOVERFLOW);

#define SUB_fixed(DEF, TEMPLATE, EXCEPT...)										\
	template <																	\
		uint r_size, int r_offset, SignMode r_sign_mode,						\
		RoundMode round_mode,													\
		uint a_size, int a_offset, SignMode a_sign_mode,						\
		uint b_size, int b_offset, SignMode b_sign_mode							\
		TEMPLATE																\
	> DEF																		\
	HWOffsetFix<r_size, r_offset, r_sign_mode>									\
	sub_fixed(	const HWOffsetFix<a_size, a_offset, a_sign_mode> &a,			\
				const HWOffsetFix<b_size, b_offset, b_sign_mode> &b ,##EXCEPT)
SUB_fixed(,HWEXCEPT,EXCEPTOVERFLOW);

#define MUL_fixed(DEF, TEMPLATE, EXCEPT...)										\
	template <																	\
		uint r_size, int r_offset, SignMode r_sign_mode,						\
		RoundMode round_mode,													\
		uint a_size, int a_offset, SignMode a_sign_mode,						\
		uint b_size, int b_offset, SignMode b_sign_mode							\
		TEMPLATE																\
	> DEF																		\
	HWOffsetFix<r_size, r_offset, r_sign_mode>									\
	mul_fixed(	const HWOffsetFix<a_size, a_offset, a_sign_mode> &a,			\
				const HWOffsetFix<b_size, b_offset, b_sign_mode> &b ,##EXCEPT)
MUL_fixed(,HWEXCEPT,EXCEPTOVERFLOW);

#define DIV_fixed(DEF, TEMPLATE, EXCEPT...)										\
	template <																	\
		uint r_size, int r_offset, SignMode r_sign_mode,						\
		RoundMode round_mode,													\
		uint a_size, int a_offset, SignMode a_sign_mode,						\
		uint b_size, int b_offset, SignMode b_sign_mode							\
		TEMPLATE																\
	> DEF																		\
	HWOffsetFix<r_size, r_offset, r_sign_mode>									\
	div_fixed(	const HWOffsetFix<a_size, a_offset, a_sign_mode> &a,			\
				const HWOffsetFix<b_size, b_offset, b_sign_mode> &b ,##EXCEPT)
DIV_fixed(,HWEXCEPT,EXCEPTFLOAT);

#define DIVMOD_fixed(DEF, TEMPLATE, EXCEPT...)                                  \
	template <                                                                  \
		uint q_size, int q_offset, SignMode q_sign_mode,                        \
		uint n_size, int n_offset, SignMode n_sign_mode,                        \
		uint d_size, int d_offset, SignMode d_sign_mode                         \
		TEMPLATE																\
	> DEF                                                                       \
	HWOffsetFix<q_size, q_offset, q_sign_mode>                                  \
	divmod_fixed(	const HWOffsetFix<n_size, n_offset, n_sign_mode> &n,        \
					const HWOffsetFix<d_size, d_offset, d_sign_mode> &d,        \
					HWOffsetFix<d_size, d_offset, d_sign_mode> *remainder ,##EXCEPT)
DIVMOD_fixed(,HWEXCEPT,EXCEPTFLOAT);

#define NEG_fixed(DEF, TEMPLATE, EXCEPT...)										\
	template <																	\
		uint r_size, int r_offset, SignMode r_sign_mode,						\
		RoundMode round_mode,													\
		uint a_size, int a_offset, SignMode a_sign_mode							\
		TEMPLATE																\
	> DEF																		\
	HWOffsetFix<r_size, r_offset, r_sign_mode>									\
	neg_fixed(const HWOffsetFix<a_size, a_offset, a_sign_mode> &a ,##EXCEPT)
NEG_fixed(,HWEXCEPT,EXCEPTOVERFLOW);


//****************************************************************************************************************************************************
// Fixed point comparison                                                                                                                            *
//****************************************************************************************************************************************************

#define EQ_fixed(DEF)															\
	template <																	\
		uint a_size, int a_offset, SignMode a_sign_mode,						\
		uint b_size, int b_offset, SignMode b_sign_mode							\
	> DEF																		\
	HWBool																		\
	eq_fixed(const HWOffsetFix<a_size, a_offset, a_sign_mode> &a,				\
			 const HWOffsetFix<b_size, b_offset, b_sign_mode> &b	)
EQ_fixed();

#define NEQ_fixed(DEF)															\
	template <																	\
		uint a_size, int a_offset, SignMode a_sign_mode,						\
		uint b_size, int b_offset, SignMode b_sign_mode							\
	> DEF																		\
	HWBool																		\
	neq_fixed(const HWOffsetFix<a_size, a_offset, a_sign_mode> &a,				\
			 const HWOffsetFix<b_size, b_offset, b_sign_mode> &b	)
NEQ_fixed();

#define GT_fixed(DEF)															\
	template <																	\
		uint a_size, int a_offset, SignMode a_sign_mode,						\
		uint b_size, int b_offset, SignMode b_sign_mode							\
	> DEF																		\
	HWBool																		\
	gt_fixed(const HWOffsetFix<a_size, a_offset, a_sign_mode> &a,				\
			 const HWOffsetFix<b_size, b_offset, b_sign_mode> &b	)
GT_fixed();

#define GTE_fixed(DEF)															\
	template <																	\
		uint a_size, int a_offset, SignMode a_sign_mode,						\
		uint b_size, int b_offset, SignMode b_sign_mode							\
	> DEF																		\
	HWBool																		\
	gte_fixed(const HWOffsetFix<a_size, a_offset, a_sign_mode> &a,				\
			 const HWOffsetFix<b_size, b_offset, b_sign_mode> &b	)
GTE_fixed();

#define LT_fixed(DEF)															\
	template <																	\
		uint a_size, int a_offset, SignMode a_sign_mode,						\
		uint b_size, int b_offset, SignMode b_sign_mode							\
	> DEF																		\
	HWBool																		\
	lt_fixed(const HWOffsetFix<a_size, a_offset, a_sign_mode> &a,				\
			 const HWOffsetFix<b_size, b_offset, b_sign_mode> &b	)
LT_fixed();

#define LTE_fixed(DEF)															\
	template <																	\
		uint a_size, int a_offset, SignMode a_sign_mode,						\
		uint b_size, int b_offset, SignMode b_sign_mode							\
	> DEF																		\
	HWBool																		\
	lte_fixed(const HWOffsetFix<a_size, a_offset, a_sign_mode> &a,				\
			 const HWOffsetFix<b_size, b_offset, b_sign_mode> &b	)
LTE_fixed();


//****************************************************************************************************************************************************
// Fixed point shifting                                                                                                                              *
//****************************************************************************************************************************************************

#define SHIFTLEFT_fixed(DEF, TEMPLATE, EXCEPT...)								\
	template <																	\
		uint a_size, int a_offset, SignMode a_sign_mode							\
		TEMPLATE																\
	> DEF																		\
	HWRawBits<a_size>															\
	shift_left_fixed(const HWOffsetFix<a_size, a_offset, a_sign_mode> &a,		\
			long shift_by ,##EXCEPT)
SHIFTLEFT_fixed(,HWEXCEPT,EXCEPTOVERFLOW);

#define SHIFTRIGHT_fixed(DEF)													\
	template <																	\
		uint a_size, int a_offset, SignMode a_sign_mode							\
	> DEF																		\
	HWRawBits<a_size>															\
	shift_right_fixed(const HWOffsetFix<a_size, a_offset, a_sign_mode> &a,		\
			long shift_by )
SHIFTRIGHT_fixed();


//****************************************************************************************************************************************************
// Fixed point bit-wise operations                                                                                                                   *
//****************************************************************************************************************************************************

#define NOT_fixed(DEF)															\
	template <																	\
		uint r_size, int r_offset, SignMode r_sign_mode							\
	> DEF																		\
	HWOffsetFix<r_size, r_offset, r_sign_mode>									\
	not_fixed(const HWOffsetFix<r_size, r_offset, r_sign_mode> &a)
NOT_fixed();

#define AND_fixed(DEF)															\
	template <																	\
		uint r_size, int r_offset, SignMode r_sign_mode							\
	> DEF																		\
	HWOffsetFix<r_size, r_offset, r_sign_mode>									\
	and_fixed(	const HWOffsetFix<r_size, r_offset, r_sign_mode> &a,			\
				const HWOffsetFix<r_size, r_offset, r_sign_mode> &b	)
AND_fixed();

#define OR_fixed(DEF)															\
	template <																	\
		uint r_size, int r_offset, SignMode r_sign_mode							\
	> DEF																		\
	HWOffsetFix<r_size, r_offset, r_sign_mode>									\
	or_fixed(	const HWOffsetFix<r_size, r_offset, r_sign_mode> &a,			\
				const HWOffsetFix<r_size, r_offset, r_sign_mode> &b	)
OR_fixed();

#define XOR_fixed(DEF)															\
	template <																	\
		uint r_size, int r_offset, SignMode r_sign_mode							\
	> DEF																		\
	HWOffsetFix<r_size, r_offset, r_sign_mode>									\
	xor_fixed(	const HWOffsetFix<r_size, r_offset, r_sign_mode> &a,			\
				const HWOffsetFix<r_size, r_offset, r_sign_mode> &b	)
XOR_fixed();



//****************************************************************************************************************************************************
// Raw bits bit-wise operations                                                                                                                      *
//****************************************************************************************************************************************************

#define NOT_bits(DEF)															\
	template <																	\
		uint r_size																\
	> DEF																		\
	HWRawBits<r_size>															\
	not_bits(const HWRawBits<r_size> &a)
NOT_bits();

#define AND_bits(DEF)															\
	template <																	\
		uint r_size																\
	> DEF																		\
	HWRawBits<r_size>															\
	and_bits(const HWRawBits<r_size> &a, const HWRawBits<r_size> &b)
AND_bits();

#define OR_bits(DEF)															\
	template <																	\
		uint r_size																\
	> DEF																		\
	HWRawBits<r_size>															\
	or_bits(const HWRawBits<r_size> &a, const HWRawBits<r_size> &b)
OR_bits();

#define XOR_bits(DEF)															\
	template <																	\
		uint r_size																\
	> DEF																		\
	HWRawBits<r_size>															\
	xor_bits(const HWRawBits<r_size> &a, const HWRawBits<r_size> &b)
XOR_bits();

#define EQ_bits(DEF)															\
	template <																	\
		uint r_size																\
	> DEF																		\
	HWBool																		\
	eq_bits(const HWRawBits<r_size> &a, const HWRawBits<r_size> &b)
EQ_bits();

#define NEQ_bits(DEF)															\
	template <																	\
		uint r_size																\
	> DEF																		\
	HWBool																		\
	neq_bits(const HWRawBits<r_size> &a, const HWRawBits<r_size> &b)
NEQ_bits();

//****************************************************************************************************************************************************
// Raw bits shifting                                                                                                                                 *
//****************************************************************************************************************************************************

#define SHIFTLEFT_bits(DEF)														\
	template <																	\
		uint a_size																\
	> DEF																		\
	HWRawBits<a_size>															\
	shift_left_bits(const HWRawBits<a_size> &a, long shift_by )
SHIFTLEFT_bits();

#define SHIFTRIGHT_bits(DEF)													\
	template <																	\
		uint a_size																\
	> DEF																		\
	HWRawBits<a_size>															\
	shift_right_bits(const HWRawBits<a_size> &a, long shift_by )
SHIFTRIGHT_bits();


//****************************************************************************************************************************************************
// Cat-ting                                                                                                                                          *
//****************************************************************************************************************************************************

#define CAT_bits_bits(DEF)														\
	template <																	\
		uint a_size,															\
		uint b_size																\
	> DEF																		\
	HWRawBits<a_size+b_size>													\
	cat(const HWRawBits<a_size> &a,												\
		const HWRawBits<b_size> &b)
CAT_bits_bits();

#define CAT_bits_float(DEF)														\
	template <																	\
		uint a_size,															\
		uint b_esize, uint b_msize												\
	> DEF																		\
	HWRawBits<a_size+b_esize+b_msize>											\
	cat(const HWRawBits<a_size> &a,												\
		const HWFloat<b_esize,b_msize> &b)
CAT_bits_float();

#define CAT_bits_fixed(DEF)														\
	template <																	\
		uint a_size,															\
		uint b_size, int b_offset, SignMode b_sign_mode							\
	> DEF																		\
	HWRawBits<a_size+b_size>													\
	cat(const HWRawBits<a_size> &a,												\
		const HWOffsetFix<b_size, b_offset, b_sign_mode> &b)
CAT_bits_fixed();

#define CAT_float_bits(DEF)														\
	template <																	\
		uint a_esize, uint a_msize,												\
		uint b_size																\
	> DEF																		\
	HWRawBits<a_esize+a_msize+b_size>											\
	cat(const HWFloat<a_esize,a_msize> &a,										\
		const HWRawBits<b_size> &b)
CAT_float_bits();

#define CAT_float_float(DEF)													\
	template <																	\
		uint a_esize, uint a_msize,												\
		uint b_esize, uint b_msize												\
	> DEF																		\
	HWRawBits<a_esize+a_msize+b_esize+b_msize>									\
	cat(const HWFloat<a_esize,a_msize> &a,										\
		const HWFloat<b_esize,b_msize> &b)
CAT_float_float();

#define CAT_float_fixed(DEF)													\
	template <																	\
		uint a_esize, uint a_msize,												\
		uint b_size, int b_offset, SignMode b_sign_mode							\
	> DEF																		\
	HWRawBits<a_esize+a_msize+b_size>											\
	cat(const HWFloat<a_esize,a_msize> &a,										\
		const HWOffsetFix<b_size, b_offset, b_sign_mode> &b)
CAT_float_fixed();

#define CAT_fixed_bits(DEF)														\
	template <																	\
		uint a_size, int a_offset, SignMode a_sign_mode,						\
		uint b_size																\
	> DEF																		\
	HWRawBits<a_size+b_size>													\
	cat(const HWOffsetFix<a_size, a_offset, a_sign_mode> &a,					\
		const HWRawBits<b_size> &b)
CAT_fixed_bits();

#define CAT_fixed_float(DEF)													\
	template <																	\
		uint a_size, int a_offset, SignMode a_sign_mode,						\
		uint b_esize, uint b_msize												\
	> DEF																		\
	HWRawBits<a_size+b_esize+b_msize>											\
	cat(const HWOffsetFix<a_size, a_offset, a_sign_mode> &a,					\
		const HWFloat<b_esize,b_msize> &b)
CAT_fixed_float();

#define CAT_fixed_fixed(DEF)													\
	template <																	\
		uint a_size, int a_offset, SignMode a_sign_mode,						\
		uint b_size, int b_offset, SignMode b_sign_mode							\
	> DEF																		\
	HWRawBits<a_size+b_size>													\
	cat(const HWOffsetFix<a_size, a_offset, a_sign_mode> &a,					\
		const HWOffsetFix<b_size, b_offset, b_sign_mode> &b)
CAT_fixed_fixed();

//****************************************************************************************************************************************************
// Slice-ing                                                                                                                                         *
//****************************************************************************************************************************************************

#define SLICE_bits(DEF)															\
	template <																	\
		uint base, uint length,													\
		uint a_size																\
	> DEF																		\
	HWRawBits<length>															\
	slice(const HWRawBits<a_size> &a)
SLICE_bits();

#define SLICE_float(DEF)														\
	template <																	\
		uint base, uint length,													\
		uint a_esize, uint a_msize												\
	> DEF																		\
	HWRawBits<length>															\
	slice(const HWFloat<a_esize,a_msize> &a)
SLICE_float();

#define SLICE_fixed(DEF)														\
	template <																	\
		uint base, uint length,													\
		uint a_size, int a_offset, SignMode a_sign_mode							\
	> DEF																		\
	HWRawBits<length>															\
	slice(const HWOffsetFix<a_size, a_offset, a_sign_mode> &a)
SLICE_fixed();

} /* end namespace maxcompilersim */

/*
 * Include helper classes (Be careful these includes need to be outside the namespace!)
 */
#include "varint_u.h"
#include "varint_s.h"
#include "HWExceptionDispatch.h"

/*
 * Include type class declarations
 */
#include "HWRawBits_decl.h"
#include "HWFloat_decl.h"
#include "HWOffsetFix_decl.h"

#ifdef SIM_USE_HWSINGLE
	#include "HWSingle_Specialization_decl.h"
#endif
#ifdef SIM_USE_HWDOUBLE
	#include "HWDouble_Specialization_decl.h"
#endif

/*
 * Include type class implementations
 */
#include "HWRawBits_impl.h"
#include "HWFloat_impl.h"
#include "HWOffsetFix_impl.h"
#ifdef SIM_USE_HWSINGLE
	#include "HWSingle_Specialization_impl.h"
#endif
#ifdef SIM_USE_HWDOUBLE
	#include "HWDouble_Specialization_impl.h"
#endif

#endif /* HWTYPES_H_ */
