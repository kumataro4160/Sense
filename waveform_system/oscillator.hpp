#pragma once

#include "effector.hpp"
#include "../wave_table_1d.hpp"
#include "../wave_table_2d.hpp"


namespace sense
{
	class Oscillator : public Effector
	{
		const float64 initialTheta;

	protected:
		float64 theta;

	public:
		Oscillator(float64 initialTheta = 0.0)noexcept :
			initialTheta(initialTheta)
		{
			reset();
		}
		void reset()noexcept override
		{
			theta = initialTheta;
		}
		void setPhase(float64 theta)noexcept
		{
			this->theta = theta;
		}
	};

	class SineOscillator : public Oscillator
	{
	public:
		SineOscillator(float64 initialTheta = 0.0)noexcept :
			Oscillator(initialTheta)
		{

		}
		__forceinline float64 process(float64 inputSample)noexcept override
		{
			if(inputSample > pi)
			{
				return 0.0;
			}
			const float64 ret = std::sin(theta);
			theta += inputSample;
			if(theta > 2.0 * pi)
			{
				theta -= 2.0 * pi;
			}
			return ret;
		}
	};

	class WaveTable1DOscillator : public Oscillator
	{
		const WaveTable1D waveTable;

	public:
		WaveTable1DOscillator(const WaveTable1D &waveTable, float64 initialTheta = 0.0) :
			Oscillator(initialTheta),
			waveTable(waveTable)
		{

		}
		__forceinline float64 process(float64 inputSample)noexcept override
		{
			if(inputSample > pi)
			{
				return 0.0;
			}
			const float64 ret = waveTable(theta);
			theta += inputSample;
			if(theta >= 2.0 * pi)
			{
				theta -= 2.0 * pi;
			}
			return ret;
		}
	};
	/*
	class WaveTable2DOscillator : public Oscillator
	{
		const std::array<Spectrum, wave_table::tableSize> spectrums;
		float64 r;

		std::array<Spectrum, wave_table::tableSize> initSpectrums(const wave_table::WaveTable2D &waveTable)
		{
			std::array<Spectrum, wave_table::tableSize> ret;
			for(uint32_t n = 0; n < wave_table::tableSize; ++n)
			{
				ret[n] = basic::convertToPolarForm(basic::fft(waveTable.getTable()[n]));
			}
			return ret;
		}

	public:
		WaveTable2DOscillator(const wave_table::WaveTable2D &waveTable)
			: spectrums(initSpectrums(waveTable))
		{

		}
		void setR(float64 r)
		{
			this->r = std::clamp(r, 0.0, 1.0);
		}
		__forceinline float64 process(float64 inputSample)override
		{
			return process(inputSample, r);
		}
		__forceinline float64 process(float64 inputSample, float64 r)
		{
			const float64 y = r * (wave_table::tableSize - 1.0);
			const int yLeft = static_cast<int>(y);
			const int yRight = (yLeft + 1) & 0x7ff;
			const float64 lRate = 0.5 + 0.5 * cos((y - yLeft) * pi);
			float64 ret = spectrums[yLeft][0].real() * lRate + spectrums[yRight][0].real() * (1.0 - lRate);
			const int N = std::min(static_cast<int>(wave_table::tableSize / 2), static_cast<int>(pi / inputSample));
			for(int n = 1; n < N; ++n)
			{
				const float64 lSample = spectrums[yLeft][n].real() * sin(n * (theta + spectrums[yLeft][n].imag())) * lRate;
				const float64 rSample = spectrums[yRight][n].real() * sin(n * (theta + spectrums[yRight][n].imag())) * (1.0 - lRate);
				ret += 2.0 * (lSample + rSample);
			}
			theta += inputSample;
			if(theta > 2.0 * pi)
			{
				theta -= 2.0 * pi;
			}
			return ret;
		}
		__forceinline float64 operator()(float64 inputSample, float64 r)
		{
			return process(inputSample, r);
		}
	};
	*/
}