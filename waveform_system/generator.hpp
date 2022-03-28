#pragma once

#include "../audio.hpp"


namespace sense
{
	class Generator
	{
	public:
		virtual void reset()noexcept = 0;
		__forceinline virtual float64 process()noexcept = 0;
		template <class signal_t>
		signal_t process(int indexFirst, int indexLast)
		{
			signal_t outputSignal(indexFirst, indexLast);
			for(int n = indexFirst; n < indexLast; ++n)
			{
				outputSignal[n] = process();
			}
			return outputSignal;
		}
		__forceinline float64 operator()()noexcept
		{
			return process();
		}
		template <class signal_t>
		signal_t operator()(int indexFirst, int indexLast)
		{
			return this->process<signal_t>(indexFirst, indexLast);
		}
	};
}