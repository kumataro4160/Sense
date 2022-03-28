#pragma once

#include "effector.hpp"
#include "../../random.hpp"
#include "../wave_table_1d.hpp"


namespace sense
{
	class SuperWaveOscillator : public Effector
	{
		const WaveTable1D waveTable;
		const std::vector<float64> initialTheta;
		const std::vector<float64> omegaRate;
		std::vector<float64> theta;

		std::vector<float64> getRandomInitialTheta(uint32_t numVoices)
		{
			std::vector<float64> ret(numVoices);
			for(auto &th : ret)
			{
				th = 2.0 * pi * getRandomNumberZeroToOne();
			}
			return ret;
		}
		std::vector<float64> getRandomOmegaRate(uint32_t numVoices, float64 detuneDepth)
		{
			std::vector<float64> ret(numVoices);
			for(auto &om : ret)
			{
				om = std::pow(2.0, detuneDepth * getRandomNumberNegativeOneToOne() / 12.0);
			}
			return ret;
		}

	public:
		SuperWaveOscillator(const WaveTable1D &waveTable, uint32_t numVoices, float64 detuneDepth)
			: waveTable(waveTable), initialTheta(getRandomInitialTheta(numVoices)), omegaRate(getRandomOmegaRate(numVoices, detuneDepth)), theta(initialTheta)
		{

		}
		void reset()noexcept override
		{
			theta = initialTheta;
		}
		__forceinline float64 process(float64 inputSample)noexcept override
		{
			float64 ret = 0.0;
			for(int n = 0; n < theta.size(); ++n)
			{
				ret += waveTable(theta[n]);
				theta[n] += inputSample * omegaRate[n];
				if(theta[n] >= 2.0 * pi)
				{
					theta[n] -= 2.0 * pi;
				}
			}
			return ret / theta.size();
		}
	};
}