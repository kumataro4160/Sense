#pragma once

#include "effector.hpp"


namespace sense
{
	class Phaser : public Effector
	{
		IIRFilter2ndOrder allPassFilter;
		const float64 depth;

	public:
		Phaser(float64 omega, float64 q, float64 depth)noexcept :
			allPassFilter(iir::allPassFilter2ndOrder(omega, q)), depth(depth)
		{

		}
		void reset()noexcept override
		{
			allPassFilter.reset();
		}
		__forceinline float64 process(float64 inputSample)noexcept override
		{
			return (1.0 - depth) * inputSample + depth * allPassFilter(inputSample);
		}
	};
}