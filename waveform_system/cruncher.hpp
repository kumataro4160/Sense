#pragma once

#include "effector.hpp"


namespace sense
{
	class Cruncher : public Effector
	{
		const float64 width;
		float64 out;

	public:
		Cruncher(float64 width)noexcept :
			width(abs(width))
		{
			reset();
		}
		void reset()noexcept override
		{
			out = 0.0;
		}
		__forceinline float64 process(float64 inputSample)noexcept override
		{
			out = std::min(std::max(inputSample - width, out), inputSample + width);
			return out;
		}
	};
}