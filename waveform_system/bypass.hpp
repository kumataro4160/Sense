#pragma once

#include "effector.hpp"


namespace sense
{
	class Bypass : public Effector
	{
	public:
		void reset()noexcept override
		{

		}
		__forceinline float64 process(float64 inputSample)noexcept override
		{
			return inputSample;
		}
	};
}