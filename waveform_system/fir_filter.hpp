#pragma once

#include "effector.hpp"
#include "../../fir/operator.hpp"

/*
namespace sense
{
	class FIRFilter : public Effector
	{
		fir::Operator coefficients;
		RingBuffer buffer;

	public:
		FIRFilter(const fir::Operator &coefficients) :
			coefficients(coefficients), buffer(coefficients.getNumSamples())
		{
			reset();
		}
		void reset()noexcept override
		{
			buffer.clear();
		}
		__forceinline float64 process(float64 inputSample)noexcept override
		{
			buffer.push(0.0);
			for(int i = 0; i < buffer.getNumSamples(); ++i)
			{
				buffer[i] += coefficients[i] * inputSample;
			}
			return buffer[0];
		}
	};
}
*/