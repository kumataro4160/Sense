#pragma once

#include "effector.hpp"


namespace sense
{
	class Shaper : public Effector
	{
		float64(*function)(float64);

	public:
		Shaper(float64(*function)(float64))noexcept
		{
			this->function = function;
		}
		void reset()noexcept override
		{

		}
		__forceinline float64 process(float64 inputSample)noexcept override
		{
			return function(inputSample);
		}
	};
}