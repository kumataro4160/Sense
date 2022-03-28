#pragma once

#include "effector.hpp"


namespace sense
{
	class Differentiator : public Effector
	{
		float64 prevInputSample;

	public:
		Differentiator()
		{
			reset();
		}
		void reset()noexcept override
		{
			prevInputSample = 0.0;
		}
		__forceinline float64 process(float64 inputSample)noexcept override
		{
			const float64 ret = inputSample = prevInputSample;
			prevInputSample = inputSample;
			return ret;
		}
	};
}