#pragma once

#include "delay.hpp"
#include "../generator/tone_generator.hpp"

/*
namespace sense
{
	class Flanger : public Effector
	{
		const float64 delayTime;
		const float64 modulationWidth;
		VariableTimeFeedbackDelay delayer;
		generator::SineToneGenerator lfo;

	public:
		Flanger(float64 delayTime, float64 feedback, float64 lfoOmega, float64 modulationWidth)noexcept :
			delayTime(delayTime),modulationWidth(modulationWidth),
			delayer(delayTime, feedback, delayTime *std::pow(2.0, modulationWidth)),
			lfo(lfoOmega)
		{
			setFeedback(feedback);
		}
		void setFeedback(float64 feedback)noexcept
		{
			delayer.setDecay(feedback);
		}
		void reset()noexcept override
		{
			delayer.reset();
			lfo.reset();
		}
		__forceinline float64 process(float64 inputSample)noexcept override
		{
			delayer.setDelayTime(delayTime * std::pow(2.0, modulationWidth * lfo()));
			return delayer(inputSample);
		}
	};
}
*/