#pragma once

#include "level_detector.hpp"


namespace sense
{
	class LevelCompressor : public Effector
	{
		const float64 threshold;
		const float64 r;
		const float64 attack;
		const uint32_t hold;
		const float64 release;
		float64 gainReduction;
		uint32_t holdCount;

	public:
		LevelCompressor(float64 threshold, float64 ratio, float64 attackTime, float64 holdTime, float64 releaseTime)noexcept :
			threshold(threshold),
			r(1.0 - 1.0 / std::max(1.0, ratio)),
			attack(1.0 / std::max(1.0, attackTime)),
			hold(std::max(0, static_cast<int>(holdTime))),
			release(1.0 / std::max(1.0, releaseTime))
		{
			reset();
		}
		void reset()noexcept override
		{
			gainReduction = 0.0;
			holdCount = 0;
		}
		__forceinline float64 process(float64 inputSample)noexcept override
		{
			const float64 inputLevel = 20.0 * std::log10(inputSample);
			const float64 difference = std::max(0.0, (inputLevel - threshold) * r);
			if(difference >= gainReduction)
			{
				gainReduction += (difference - gainReduction) * attack;
				holdCount = hold;
			}
			else
			{
				if(holdCount > 0)
				{
					holdCount--;
				}
				else
				{
					gainReduction -= (gainReduction - difference) * release;
				}
			}
			return std::pow(10.0, -gainReduction / 20.0);
		}
	};

	class LevelLimiter : public Effector
	{
		const float64 threshold;
		const uint32_t hold;
		const float64 release;
		float64 gainReduction;
		uint32_t holdCount;

	public:
		LevelLimiter(float64 threshold, float64 holdTime, float64 releaseTime)noexcept :
			threshold(threshold),
			hold(std::max(0, static_cast<int>(holdTime))),
			release(1.0 / std::max(1.0, releaseTime))
		{
			reset();
		}
		void reset()noexcept override
		{
			gainReduction = 0.0;
			holdCount = 0;
		}
		__forceinline float64 process(float64 inputSample)noexcept override
		{
			const float64 inputLevel = 20.0 * std::log10(inputSample);
			const float64 difference = std::max(0.0, inputLevel - threshold);
			if(difference >= gainReduction)
			{
				gainReduction = difference;
				holdCount = hold;
			}
			else
			{
				if(holdCount > 0)
				{
					holdCount--;
				}
				else
				{
					gainReduction -= (gainReduction - difference) * release;
				}
			}
			return std::pow(10.0, -gainReduction / 20.0);
		}
	};

	class LevelExpander : public Effector
	{
		const float64 threshold;
		const float64 range;
		const float64 r;
		const float64 attack;
		const uint32_t hold;
		const float64 release;
		float64 gainReduction;
		uint32_t holdCount;

	public:
		LevelExpander(float64 threshold, float64 range, float64 ratio, float64 attackTime, float64 holdTime, float64 releaseTime)noexcept :
			threshold(threshold),
			range(std::min(0.0, range)),
			r(std::max(1.0, ratio) - 1.0),
			attack(1.0 / std::max(1.0, attackTime)),
			hold(std::max(0, static_cast<int>(holdTime))),
			release(1.0 / std::max(1.0, releaseTime))
		{
			reset();
		}
		void reset()noexcept override
		{
			gainReduction = -range;
			holdCount = 0;
		}
		__forceinline float64 process(float64 inputSample)noexcept override
		{
			const float64 inputLevel = 20.0 * std::log10(inputSample);
			const float64 difference = std::clamp((threshold - inputLevel) * r, 0.0, -range);
			if(difference <= gainReduction)
			{
				gainReduction -= (gainReduction - difference) * attack;
				holdCount = hold;
			}
			else
			{
				if(holdCount > 0)
				{
					holdCount--;
				}
				else
				{
					gainReduction += (difference - gainReduction) * release;
				}
			}
			return std::pow(10.0, -gainReduction / 20.0);
		}
	};

	class LevelGater : public Effector
	{
		const float64 threshold;
		const float64 range;
		const float64 attack;
		const uint32_t hold;
		const float64 release;
		float64 gainReduction;
		uint32_t holdCount;

	public:
		LevelGater(float64 threshold, float64 range, float64 attackTime, float64 holdTime, float64 releaseTime)noexcept :
			threshold(threshold),
			range(std::min(0.0, range)),
			attack(1.0 / std::max(1.0, attackTime)),
			hold(std::max(0, static_cast<int>(holdTime))),
			release(1.0 / std::max(1.0, releaseTime))
		{
			reset();
		}
		void reset()noexcept override
		{
			gainReduction = -range;
			holdCount = 0;
		}
		__forceinline float64 process(float64 inputSample)noexcept override
		{
			const float64 inputLevel = 20.0 * std::log10(inputSample);
			const float64 difference = inputLevel < threshold ? -range : 0.0;
			if(difference <= gainReduction)
			{
				gainReduction -= (gainReduction - difference) * attack;
				holdCount = hold;
			}
			else
			{
				if(holdCount > 0)
				{
					holdCount--;
				}
				else
				{
					gainReduction += (std::min(difference, -range) - gainReduction) * release;
				}
			}
			return std::pow(10.0, -gainReduction / 20.0);
		}
	};

	class DynamicsProcessor : public Effector
	{
		LevelDetector &levelDetector;
		Effector &levelShaper;

	public:
		DynamicsProcessor(LevelDetector &levelDetector, Effector &levelShaper)noexcept :
			levelDetector(levelDetector), levelShaper(levelShaper)
		{

		}
		void reset()noexcept override
		{
			levelDetector.reset();
			levelShaper.reset();
		}
		__forceinline float64 process(float64 inputSample)noexcept override
		{
			return sidechainProcess(inputSample, inputSample);
		}
		__forceinline float64 sidechainProcess(float64 inputSample, float64 sidechain)noexcept
		{
			return inputSample * levelShaper.process(levelDetector.process(sidechain));
		}
	};
}