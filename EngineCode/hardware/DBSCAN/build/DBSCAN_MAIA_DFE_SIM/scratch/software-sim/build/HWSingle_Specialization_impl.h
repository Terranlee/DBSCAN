#ifndef HWSINGLE_SPECIALIZATION_IMPL_H_
#define HWSINGLE_SPECIALIZATION_IMPL_H_

#include "HWSingle_Specialization_decl.h"

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
HWSingle HWFloat<8,24>::add_float(const HWSingle &b, EXCEPTFLOAT) const
{
	#pragma STDC FENV_ACCESS ON

	CLEAR_EXCEPTION_MASK(hw_exception);
	float f_result = m_value + b.m_value;
	CHECK_INVALID_OPERATION(hw_exception,exception);

	HWFloat<8,24> result (f_result);
	return result;
}

template<bool hw_exception> inline
HWSingle HWFloat<8,24>::sub_float(const HWSingle &subtrahend, EXCEPTFLOAT) const
{
	#pragma STDC FENV_ACCESS ON

	CLEAR_EXCEPTION_MASK(hw_exception);
	float f_result = m_value - subtrahend.m_value;
	CHECK_INVALID_OPERATION(hw_exception,exception);

	HWFloat<8,24> result (f_result);
	return result;
}

template<bool hw_exception> inline
HWSingle HWFloat<8,24>::mul_float(const HWSingle &multiplicand, EXCEPTFLOAT) const
{
	#pragma STDC FENV_ACCESS ON

	CLEAR_EXCEPTION_MASK(hw_exception);
	float f_result = m_value * multiplicand.m_value;
	CHECK_INVALID_OPERATION(hw_exception,exception);

	HWFloat<8,24> result (f_result);
	return result;
}

template<bool hw_exception> inline
HWSingle HWFloat<8,24>::div_float(const HWSingle &dividend, EXCEPTFLOAT) const
{
	#pragma STDC FENV_ACCESS ON


	if (ISNAN(m_value) || ISNAN(dividend.m_value)) {
		if (hw_exception)
			*exception = ExceptFloat(0);
		return HWSingle(NAN);
	}


	CLEAR_EXCEPTION_MASK(hw_exception);
	float f_result = m_value / dividend.m_value;
	CHECK_INVALID_OPERATION_OR_ZERO_DIVISON(hw_exception,exception);

	HWFloat<8,24> result (f_result);
	return result;
}

template<bool hw_exception> inline
HWSingle HWFloat<8,24>::sqrt_float(EXCEPTFLOAT) const
{
	#pragma STDC FENV_ACCESS ON

	if (ISNAN(m_value))
	{
		if (hw_exception)
			*exception = ExceptFloat(0);
		return HWSingle(NAN);
	}

	CLEAR_EXCEPTION_MASK(hw_exception);
	float f_result = sqrt(m_value);
	CHECK_INVALID_OPERATION(hw_exception,exception);

	HWFloat<8,24> result (f_result);
	return result;
}

} /* end namespace maxcompilersim */

#endif /* HWSINGLE_SPECIALIZATION_IMPL_H_ */
