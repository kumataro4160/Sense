#pragma once

#include "effector.hpp"


namespace sense
{
	class Crusher : public Effector
	{
		const unsigned downsample;
		unsigned count;
		float64 prevSample;

	public:
		Crusher(unsigned downsample)noexcept :
			downsample(downsample)
		{
			reset();
		}
		void reset()noexcept override
		{
			count = 0;
			prevSample = 0.0;
		}
		__forceinline float64 process(float64 inputSample)noexcept override
		{
			if(count == downsample)
			{
				prevSample = inputSample;
				count = 0;
			}
			++count;
			return prevSample;
		}
	};
}