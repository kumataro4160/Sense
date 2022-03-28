#pragma once

#include "effector.hpp"


namespace sense
{
	class PitchDetector : public Effector
	{


	public:
		PitchDetector()noexcept
		{
			reset();
		}
		void reset()noexcept override
		{

		}
		__forceinline float64 process(float64 inputSample)noexcept override
		{

		}
	};
}