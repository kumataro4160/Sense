#pragma once

#include "delay.hpp"

/*
namespace sense
{
	class CombFilter : public Effector
	{
		Delay delayer;

	public:
		CombFilter(float64 frequency) :
			delayer(1.0 / frequency)
		{

		}
		void reset()noexcept override
		{
			delayer.reset();
		}
		__forceinline float64 process(float64 inputSample)noexcept override
		{
			return inputSample + delayer(inputSample);
		}
	};

	class VariableFrequencyCombFilter : public Effector
	{
		VariableTimeDelay delayer;

	public:
		VariableFrequencyCombFilter(float64 frequency, float64 minFrequency) :
			delayer(1.0 / frequency, 1.0 / minFrequency)
		{

		}
		void reset()noexcept override
		{
			delayer.reset();
		}
		__forceinline float64 process(float64 inputSample)noexcept override
		{
			return inputSample + delayer(inputSample);
		}
	};
}
*/