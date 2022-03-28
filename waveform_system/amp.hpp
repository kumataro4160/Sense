#pragma once

#include "effector.hpp"


namespace sense
{
	class Amp : public Effector
	{
		float64(*const function)(float64);
		float64 prevOut;

	public:
		Amp(float64(*function)(float64))noexcept :
			function(function)
		{
			reset();
		}
		void reset()noexcept override
		{
			prevOut = 0.0;
		}
		__forceinline float64 process(float64 inputSample)noexcept override
		{
			return prevOut = prevOut + function(inputSample - prevOut);
		}
	};
}