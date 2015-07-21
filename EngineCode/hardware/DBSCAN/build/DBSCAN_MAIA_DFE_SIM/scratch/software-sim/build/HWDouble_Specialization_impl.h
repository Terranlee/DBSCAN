#ifndef HWDOUBLE_SPECIALIZATION_IMPL_H_
#define HWDOUBLE_SPECIALIZATION_IMPL_H_

#include "HWDouble_Specialization_decl.h"

#define MAX(a, b) ( ((a)>(b)) ? (a) : (b) )
#define MIN(a, b) ( ((a)>(b)) ? (b) : (a) )


namespace maxcompilersim {


// Arithmetic ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define CLEAR_EXCEPTION_MASK(enabled)       \
	if (enabled)                            \
		assert(feclearexcept(FE_ALL_EXCEPT))

#define CHECK_INVALID_OPERATION(enabled,exception)      \
	if (enabled)                                        \
	{                                                   \
		varint_u<4> except_vector;                      \
		if (fetestexcept(FE_INVALID))                   \
			except_vector[2] = true;                    \
		*exception = ExceptFloat(except_vector);        \
	}

#define CHECK_INVALID_OPERATION_OR_ZERO_DIVISON(enabled,exception) \
	if (enabled)                                        \
	{                                                   \
		varint_u<4> except_vector;                      \
		if (fetestexcept(FE_INVALID))                   \
			except_vector[2] = true;                    \
		if (fetestexcept(FE_DIVBYZERO))                 \
			except_vector[3] = true;                    \
		*exception = ExceptFloat(except_vector);        \
	}

template<bool hw_exception> inline
HWDouble HWFloat<DOUBLE_EXPONENT_SIZE,DOUBLE_MANTISSA_SIZE>::add_float(const HWDouble &b, EXCEPTFLOAT) const
{
	#pragma STDC FENV_ACCESS ON

	CLEAR_EXCEPTION_MASK(hw_exception);
	double f_result = m_value + b.m_value;
	CHECK_INVALID_OPERATION(hw_exception,exception);

	HWFloat<DOUBLE_EXPONENT_SIZE,DOUBLE_MANTISSA_SIZE> result (f_result);
	return result;
}

template<bool hw_exception> inline
HWDouble HWFloat<DOUBLE_EXPONENT_SIZE,DOUBLE_MANTISSA_SIZE>::sub_float(const HWDouble &subtrahend, EXCEPTFLOAT) const
{
	#pragma STDC FENV_ACCESS ON

	CLEAR_EXCEPTION_MASK(hw_exception);
	double f_result = m_value - subtrahend.m_value;
	CHECK_INVALID_OPERATION(hw_exception,exception);

	HWFloat<DOUBLE_EXPONENT_SIZE,DOUBLE_MANTISSA_SIZE> result (f_result);
	return result;
}

template<bool hw_exception> inline
HWDouble HWFloat<DOUBLE_EXPONENT_SIZE,DOUBLE_MANTISSA_SIZE>::mul_float(const HWDouble &multiplicand, EXCEPTFLOAT) const
{
	#pragma STDC FENV_ACCESS ON

	CLEAR_EXCEPTION_MASK(hw_exception);
	double f_result = m_value * multiplicand.m_value;
	CHECK_INVALID_OPERATION(hw_exception,exception);

	HWFloat<DOUBLE_EXPONENT_SIZE,DOUBLE_MANTISSA_SIZE> result (f_result);
	return result;
}

template<bool hw_exception> inline
HWDouble HWFloat<DOUBLE_EXPONENT_SIZE,DOUBLE_MANTISSA_SIZE>::div_float(const HWDouble &dividend, EXCEPTFLOAT) const
{
	#pragma STDC FENV_ACCESS ON


	if (ISNAN(m_value) || ISNAN(dividend.m_value)) {
		if (hw_exception)
			*exception = ExceptFloat(0);
		return HWDouble(NAN);
	}


	CLEAR_EXCEPTION_MASK(hw_exception);
	double f_result = m_value / dividend.m_value;
	CHECK_INVALID_OPERATION_OR_ZERO_DIVISON(hw_exception,exception);

	HWFloat<DOUBLE_EXPONENT_SIZE,DOUBLE_MANTISSA_SIZE> result (f_result);
	return result;
}

template<bool hw_exception> inline
HWDouble HWFloat<DOUBLE_EXPONENT_SIZE,DOUBLE_MANTISSA_SIZE>::sqrt_float(EXCEPTFLOAT) const
{
	#pragma STDC FENV_ACCESS ON

	if (ISNAN(m_value))
	{
		if (hw_exception)
			*exception = ExceptFloat(0);
		return HWDouble(NAN);
	}

	CLEAR_EXCEPTION_MASK(hw_exception);
	double f_result = sqrt(m_value);
	CHECK_INVALID_OPERATION(hw_exception,exception);

	HWFloat<DOUBLE_EXPONENT_SIZE,DOUBLE_MANTISSA_SIZE> result (f_result);
	return result;
}

} /* end namespace maxcompilersim */

#endif /* HWDOUBLE_SPECIALIZATION_IMPL_H_ */
