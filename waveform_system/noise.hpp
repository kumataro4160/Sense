#pragma once

#include "generator.hpp"
#include "../../random.hpp"
#include "../effector/iir_filter.hpp"


namespace sense
{
	class WhiteNoiseGenerator : public Generator
	{
	public:
		void reset()noexcept override
		{

		}
		__forceinline float64 process()noexcept override
		{
			return getRandomNumberNormalDistribution();
		}
	};

	class PurpleNoiseGenerator : public Generator
	{
		WhiteNoiseGenerator whiteNoiseGenerator;
		IIRFilter1stOrder highPassFilter;

	public:
		PurpleNoiseGenerator()noexcept :
			highPassFilter(iir::highPassFilter1stOrder(pi * 0.6))
		{

		}
		void reset()noexcept override
		{
			whiteNoiseGenerator.reset();
			highPassFilter.reset();
		}
		__forceinline float64 process()noexcept override
		{
			return highPassFilter(whiteNoiseGenerator());
		}
	};

	class BlueNoiseGenerator : public Generator
	{
		WhiteNoiseGenerator whiteNoiseGenerator;
		IIRFilter2ndOrder lowShelfFilter;

	public:
		BlueNoiseGenerator()noexcept :
			lowShelfFilter(iir::lowShelfFilter2ndOrderQ(pi * 0.04, -40.0, 0.23))
		{

		}
		void reset()noexcept override
		{
			whiteNoiseGenerator.reset();
			lowShelfFilter.reset();
		}
		__forceinline float64 process()noexcept override
		{
			return lowShelfFilter(whiteNoiseGenerator());
		}
	};

	class PinkNoiseGenerator : public Generator
	{
		WhiteNoiseGenerator whiteNoiseGenerator;
		IIRFilter2ndOrder highShelfFilter;

	public:
		PinkNoiseGenerator()noexcept :
			highShelfFilter(iir::highShelfFilter2ndOrderQ(pi * 0.04, -40.0, 0.23))
		{

		}
		void reset()noexcept override
		{
			whiteNoiseGenerator.reset();
			highShelfFilter.reset();
		}
		__forceinline float64 process()noexcept override
		{
			return highShelfFilter(whiteNoiseGenerator());
		}
	};

	class RedNoiseGenerator : public Generator
	{
		WhiteNoiseGenerator whiteNoiseGenerator;
		IIRFilter1stOrder lowPassFilter;
		IIRFilter1stOrder highPassFilter;

	public:
		RedNoiseGenerator()noexcept :
			lowPassFilter(iir::lowPassFilter1stOrder(pi * 0.0001)), highPassFilter(iir::highPassFilter1stOrder(pi * 0.0001))
		{

		}
		void reset()noexcept override
		{
			whiteNoiseGenerator.reset();
			lowPassFilter.reset();
			highPassFilter.reset();
		}
		__forceinline float64 process()noexcept override
		{
			return highPassFilter(lowPassFilter(whiteNoiseGenerator())) * 80.0;
		}
	};
}