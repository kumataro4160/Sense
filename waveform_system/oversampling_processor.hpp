#pragma once

#include "effector.hpp"
#include "iir_filter.hpp"
#include "../../iir/filter_2nd_order.hpp"


namespace sense
{
	class OversamplingProcessor : public Effector
	{
		const uint32_t quality;
		Effector &effector;
		IIRFilter2ndOrder upsamplingFilter1;
		IIRFilter2ndOrder upsamplingFilter2;
		IIRFilter2ndOrder upsamplingFilter3;
		IIRFilter2ndOrder upsamplingFilter4;
		IIRFilter2ndOrder downsamplingFilter1;
		IIRFilter2ndOrder downsamplingFilter2;
		IIRFilter2ndOrder downsamplingFilter3;
		IIRFilter2ndOrder downsamplingFilter4;

		__forceinline float64 upsample(float64 inputSample)noexcept
		{
			return upsamplingFilter4(upsamplingFilter3(upsamplingFilter2(upsamplingFilter1(inputSample))));
		}
		__forceinline float64 downsample(float64 inputSample)noexcept
		{
			return downsamplingFilter4(downsamplingFilter3(downsamplingFilter2(downsamplingFilter1(inputSample))));
		}

	public:
		OversamplingProcessor(uint32_t quality, Effector &effector)noexcept :
			quality(quality), effector(effector),
			upsamplingFilter1(iir::lowPassFilter2ndOrder(0.5 * pi / quality, sqrt(0.5))),
			upsamplingFilter2(iir::lowPassFilter2ndOrder(0.5 * pi / quality, sqrt(0.5))),
			upsamplingFilter3(iir::lowPassFilter2ndOrder(0.5 * pi / quality, sqrt(0.5))),
			upsamplingFilter4(iir::lowPassFilter2ndOrder(0.5 * pi / quality, sqrt(0.5))),
			downsamplingFilter1(iir::lowPassFilter2ndOrder(0.5 * pi / quality, sqrt(0.5))),
			downsamplingFilter2(iir::lowPassFilter2ndOrder(0.5 * pi / quality, sqrt(0.5))),
			downsamplingFilter3(iir::lowPassFilter2ndOrder(0.5 * pi / quality, sqrt(0.5))),
			downsamplingFilter4(iir::lowPassFilter2ndOrder(0.5 * pi / quality, sqrt(0.5)))
		{

		}
		void reset()noexcept override
		{

		}
		__forceinline float64 process(float64 inputSample)noexcept override
		{
			const float64 ret = downsample(effector(upsample(inputSample * quality)));
			for(uint32_t i = 1; i < quality; ++i)
			{
				downsample(effector(upsample(0.0)));
			}
			return ret;
		}
	};
}