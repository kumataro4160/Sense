#pragma once

#include "generator.hpp"
#include "../effector/iir_filter.hpp"
#include "../../iir/oscillation.hpp"
#include "../wave_table_2d.hpp"


namespace sense
{
	class ToneGenerator : public Generator
	{
	protected:
		const float64 initialTheta;
		const float64 omega;
		float64 theta;

	public:
		ToneGenerator(float64 omega, float64 initialTheta = 0.0)noexcept :
			initialTheta(initialTheta), omega(omega)
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

	class IIRSineToneGenerator : public Generator
	{
		IIRFilter2ndOrder filter;
		float64 prev;

	public:
		IIRSineToneGenerator(float64 omega)noexcept :
			filter(iir::sine(omega))
		{
			reset();
		}
		void reset()noexcept override
		{
			filter.reset();
			prev = 1.0;
		}
		__forceinline float64 process()noexcept override
		{
			const float64 ret = filter(prev);
			prev = 0.0;
			return ret;
		}
	};

	class SineToneGenerator : public ToneGenerator
	{
	public:
		SineToneGenerator(float64 omega, float64 initialTheta = 0.0)noexcept :
			ToneGenerator(omega, initialTheta)
		{

		}
		__forceinline float64 process()noexcept override
		{
			if(omega > pi)
			{
				return 0.0;
			}
			const float64 ret = sin(theta);
			theta += omega;
			if(theta > 2.0 * pi)
			{
				theta -= 2.0 * pi;
			}
			return ret;
		}
	};

	class WaveTable1DToneGenerator : public ToneGenerator
	{
		const WaveTable1D waveTable;

	public:
		WaveTable1DToneGenerator(float64 omega, const WaveTable1D &waveTable, float64 initialTheta = 0.0) :
			ToneGenerator(omega, initialTheta),
			waveTable(waveTable)
		{

		}
		__forceinline float64 process()noexcept override
		{
			if(omega > pi)
			{
				return 0.0;
			}
			const float64 ret = waveTable(theta);
			theta += omega;
			if(theta > 2.0 * pi)
			{
				theta -= 2.0 * pi;
			}
			return ret;
		}
	};
	/*
	class WaveTable2DToneGenerator : public ToneGenerator
	{
		const std::vector<spectrum::SpectrumRectangular> spectrums;
		float64 r;

		std::vector<spectrum::SpectrumRectangular> initSpectrums(float64 omega, const wave_table::WaveTable2D &waveTable, float64 initialTheta = 0.0)
		{
			std::vector<spectrum::SpectrumRectangular> ret;
			for(uint32_t n = 0; n < wave_table::tableSize; ++n)
			{
				ret.emplace_back(Container<polar>(convertToPolarForm(fft(waveTable.getTable()[n])).getContainer()).resize(std::min(static_cast<int>(wave_table::tableSize / 2), static_cast<int>(pi / omega))));
			}
			return ret;
		}

	public:
		WaveTable2DToneGenerator(float64 omega, const wave_table::WaveTable2D &waveTable) :
			ToneGenerator(omega, initialTheta), spectrums(initSpectrums(omega, waveTable))
		{

		}
		void setR(float64 r)
		{
			this->r = std::clamp(r, 0.0, 1.0);
		}
		__forceinline float64 process()override
		{
			return process(r);
		}
		__forceinline float64 process(float64 r)
		{
			if(omega > pi)
			{
				return 0.0;
			}
			const float64 y = std::clamp(r * (wave_table::tableSize - 1.0), 0.0, wave_table::tableSize - 1.0);
			const int yLeft = static_cast<int>(y);
			const int yRight = (yLeft + 1) & 0x7ff;
			const float64 lRate = 0.5 + 0.5 * cos((y - yLeft) * pi);
			float64 ret = spectrums[yLeft][0].real() * lRate + spectrums[yRight][0].real() * (1.0 - lRate);
			const int N = spectrums[0].getNumSamples();
			for(int n = 1; n < N; ++n)
			{
				const float64 lSample = spectrums[yLeft][n].real() * sin(n * (theta + spectrums[yLeft][n].imag())) * lRate;
				const float64 rSample = spectrums[yRight][n].real() * sin(n * (theta + spectrums[yRight][n].imag())) * (1.0 - lRate);
				ret += 2.0 * (lSample + rSample);
			}
			theta += omega;
			if(theta > 2.0 * pi)
			{
				theta -= 2.0 * pi;
			}
			return ret;
		}
		__forceinline float64 operator()(float64 r)
		{
			return process(r);
		}
	};
	*/
}