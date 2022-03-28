#pragma once

#include "effector.hpp"
#include "iir_filter.hpp"


namespace sense
{
	class Octaver : public Effector
	{
		const uint32_t quality;
		IIRFilter2ndOrder fundamentalFrequencyDetectionFilter;
		char prevDigitalSignal;
		bool outputs1;
		IIRFilter2ndOrder downsamplingFilter1;
		IIRFilter2ndOrder downsamplingFilter2;

		constexpr static char digitalize(float64 x)noexcept
		{
			return x > 0.0 ? 1 : -1;
		}
		__forceinline float64 output(float64 inputSample)noexcept
		{
			const char digitalSignal = digitalize(fundamentalFrequencyDetectionFilter(inputSample));
			const char difference = digitalSignal - prevDigitalSignal;
			prevDigitalSignal = digitalSignal;
			if(difference > 0)
			{
				outputs1 = !outputs1;
				return outputs1 ? 1.0 : 0.0;
			}
			return 0.0;
		}
		__forceinline float64 downsample(float64 inputSample)noexcept
		{
			return downsamplingFilter2(downsamplingFilter1(inputSample));
		}

	public:
		Octaver(uint32_t quality) :
			quality(quality),
			fundamentalFrequencyDetectionFilter(iir::lowPassFilter2ndOrder(2.0 * pi / 1000.0 / quality, 0.5)),
			downsamplingFilter1(iir::lowPassFilter2ndOrder(0.5 * pi / quality, sqrt(0.5))),
			downsamplingFilter2(iir::lowPassFilter2ndOrder(0.5 * pi / quality, sqrt(0.5)))
		{
			reset();
		}
		void reset()noexcept override
		{
			fundamentalFrequencyDetectionFilter.reset();
			prevDigitalSignal = 0;
			outputs1 = false;
			downsamplingFilter1.reset();
			downsamplingFilter2.reset();
		}
		__forceinline float64 process(float64 inputSample)noexcept override
		{
			const float64 ret = downsample(output(inputSample));
			for(uint32_t i = 1; i < quality; ++i)
			{
				downsample(output(0.0));
			}
			return ret;
		}
	};
}