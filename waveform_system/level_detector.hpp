#pragma once

#include "shaper.hpp"
#include "iir_filter.hpp"


namespace sense
{
	class LevelDetector : public Effector
	{

	};

	class PeakLevelDetector : public LevelDetector
	{
		Shaper shaper;

	public:
		PeakLevelDetector()noexcept :
			shaper(std::abs)
		{

		}
		void reset()noexcept override
		{
			shaper.reset();
		}
		__forceinline float64 process(float64 inputSample)noexcept override
		{
			return shaper(inputSample);
		}
	};

	class LowPassLevelDetector : public LevelDetector
	{
		PeakLevelDetector peakLevelDetector;
		IIRFilterCascaded filter;

	public:
		LowPassLevelDetector(float64 omega) :
			filter(iir::lowPassFilter2ndOrder(omega, 0.5) * iir::lowPassFilter2ndOrder(omega, 0.5))
		{

		}
		void reset()noexcept override
		{
			peakLevelDetector.reset();
			filter.reset();
		}
		__forceinline float64 process(float64 inputSample)noexcept override
		{
			return filter(peakLevelDetector(inputSample)) * (pi / 2.0);
		}
	};
}