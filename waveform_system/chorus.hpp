#pragma once

#include "delay.hpp"
#include "../generator/tone_generator.hpp"

/*
namespace sense
{
	class Chorus : public Effector
	{
		const uint32_t numVoices;
		const float64 delayTime;
		const float64 modulationWidth;
		std::vector<VariableTimeDelay> delayers;
		std::vector<generator::SineToneGenerator> lfos;

	public:
		Chorus(uint32_t numVoices, float64 delayTime, float64 lfoOmega, float64 modulationWidth) :
			numVoices(numVoices), delayTime(delayTime), modulationWidth(modulationWidth)
		{
			for(uint32_t i = 0; i < numVoices; ++i)
			{
				delayers.emplace_back(delayTime * (i + 1.0) / numVoices, delayTime * (i + 1.0) / numVoices * std::pow(2.0, abs(modulationWidth) / 12.0));
				lfos.emplace_back(lfoOmega, 2.0 * pi * i / numVoices);
			}
		}
		void reset()noexcept override
		{
			for(VariableTimeDelay &delayer : delayers)
			{
				delayer.reset();
			}
			for(generator::SineToneGenerator &lfo : lfos)
			{
				lfo.reset();
			}
		}
		__forceinline float64 process(float64 inputSample)noexcept override
		{
			for(uint32_t i = 0; i < numVoices; ++i)
			{
				delayers[i].setDelayTime(delayTime * (i + 1.0) / numVoices * std::pow(2.0, modulationWidth * lfos[i]() / 12.0));
			}
			float64 ret = 0.0;
			for(VariableTimeDelay &delayer : delayers)
			{
				ret += delayer(inputSample);
			}
			return ret / numVoices;
		}
	};
}
*/