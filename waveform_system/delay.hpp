#pragma once

#include "effector.hpp"
#include "../fixedlength_ring_buffer.hpp"
#include "../../interpolation.hpp"

/*
namespace sense
{
	class Delay : public Effector
	{
		const uint32_t precision;
		const float64 n;
		RingBuffer ringBuffer;

	public:
		Delay(float64 time) :
			precision(std::min(10U, static_cast<uint32_t>(time) + 1)),
			n(static_cast<int>(time) - time + precision), ringBuffer(static_cast<int>(time) + precision + 1)
		{

		}
		void reset()noexcept override
		{
			ringBuffer.clear();
		}
		__forceinline float64 process(float64 inputSample)noexcept override
		{
			ringBuffer.push(inputSample);
			return interpolateSinc(ringBuffer, n, precision);
		}
	};

	class FeedbackDelay : public Effector
	{
		const uint32_t precision;
		const float64 n;
		float64 decay;
		RingBuffer ringBuffer;

	public:
		FeedbackDelay(float64 time, float64 decay) :
			precision(std::min(10U, static_cast<uint32_t>(time) + 1)),
			n(static_cast<int>(time) - time + precision), ringBuffer(static_cast<int>(time) + precision)
		{
			setDecay(decay);
		}
		void setDecay(float64 decay)noexcept
		{
			this->decay = decay;
		}
		void reset()noexcept override
		{
			ringBuffer.clear();
		}
		__forceinline float64 process(float64 inputSample)noexcept override
		{
			const float64 ret = decay * interpolateSinc(ringBuffer, n, precision);
			ringBuffer.push(inputSample + ret);
			return ret;
		}
	};

	class FeedbackEffectDelay : public Effector
	{
		const uint32_t precision;
		const float64 n;
		float64 decay;
		RingBuffer ringBuffer;
		Effector &effector;

	public:
		FeedbackEffectDelay(float64 time, float64 decay, Effector &effector) :
			precision(std::min(10U, static_cast<uint32_t>(time) + 1)),
			n(static_cast<int>(time) - time + precision), ringBuffer(static_cast<int>(time) + precision),
			effector(effector)
		{
			setDecay(decay);
		}
		void setDecay(float64 decay)noexcept
		{
			this->decay = decay;
		}
		void reset()noexcept override
		{
			ringBuffer.clear();
		}
		__forceinline float64 process(float64 inputSample)noexcept override
		{
			const float64 ret = decay * interpolateSinc(ringBuffer, n, precision);
			ringBuffer.push(inputSample + ret);
			return ret;
		}
	};

	class VariableTimeDelay : public Effector
	{
		const float64 maxTime;
		const uint32_t precision;
		float64 n;
		RingBuffer ringBuffer;

	public:
		VariableTimeDelay(float64 time, float64 maxTime) :
			maxTime(maxTime), precision(std::min(10U, static_cast<uint32_t>(maxTime) + 1)),
			ringBuffer(static_cast<int>(maxTime) + precision + 1)
		{
			setDelayTime(time);
		}
		void setDelayTime(float64 time)noexcept
		{
			if(time < 0.0)
			{
				return;
			}
			n = static_cast<uint32_t>(maxTime) - time + precision;
		}
		void reset()noexcept override
		{
			ringBuffer.clear();
		}
		__forceinline float64 process(float64 inputSample)noexcept override
		{
			ringBuffer.push(inputSample);
			return interpolateSinc(ringBuffer, n, precision);
		}
	};

	class VariableTimeFeedbackDelay : public Effector
	{
		const float64 maxTime;
		const uint32_t precision;
		float64 n;
		float64 decay;
		RingBuffer ringBuffer;

	public:
		VariableTimeFeedbackDelay(float64 time, float64 decay, float64 maxTime) :
			maxTime(maxTime), precision(std::min(10U, static_cast<uint32_t>(maxTime) + 1)),
			ringBuffer(static_cast<int>(maxTime) + precision)
		{
			setDelayTime(time);
			setDecay(decay);
		}
		void setDelayTime(float64 time)noexcept
		{
			if(time < 0.0)
			{
				return;
			}
			n = static_cast<uint32_t>(maxTime) - time + precision;
		}
		void setDecay(float64 decay)noexcept
		{
			this->decay = decay;
		}
		void reset()noexcept override
		{
			ringBuffer.clear();
		}
		__forceinline float64 process(float64 inputSample)noexcept override
		{
			float64 ret = decay * interpolateSinc(ringBuffer, n, precision);
			ringBuffer.push(inputSample + ret);
			return ret;
		}
	};
}
*/