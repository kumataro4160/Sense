#pragma once

#include "../audio.hpp"


namespace sense
{
	class Effector
	{
	public:
		virtual void reset()noexcept = 0;
		__forceinline virtual float64 process(float64 inputSample)noexcept = 0;
		template <class signal_t>
		signal_t process(const signal_t &inputSignal)
		{
			signal_t outputSignal(inputSignal.getFirstTime(), inputSignal.getLastTime());
			for(longlong n = inputSignal.getFirstTime(); n < inputSignal.getLastTime(); ++n)
			{
				outputSignal[n] = process(inputSignal[n]);
			}
			return outputSignal;
		}
		__forceinline float64 operator()(float64 inputSample)noexcept
		{
			return process(inputSample);
		}
		template <class signal_t>
		signal_t operator()(const signal_t &inputSignal)
		{
			return process(inputSignal);
		}
	};
}