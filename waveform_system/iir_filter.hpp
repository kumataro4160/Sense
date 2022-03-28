#pragma once

#include "effector.hpp"
#include "../../iir/filter_1st_order.hpp"
#include "../../iir/filter_2nd_order.hpp"


namespace sense
{
	class IIRFilter1stOrder : public Effector
	{
		const float64 a1;
		const float64 b0;
		const float64 b1;
		float64 in1;
		float64 out1;

	public:
		IIRFilter1stOrder(const iir::Coefficients1stOrder &coefficients)noexcept :
			a1(coefficients.a1), b0(coefficients.b0), b1(coefficients.b1)
		{
			reset();
		}
		IIRFilter1stOrder(float64 a1, float64 b0, float64 b1)noexcept :
			a1(a1), b0(b0), b1(b1)
		{
			reset();
		}
		IIRFilter1stOrder(float64 a0, float64 a1, float64 b0, float64 b1)noexcept :
			a1(a1 / a0), b0(b0 / a0), b1(b1 / a0)
		{
			reset();
		}
		void reset()noexcept override
		{
			in1 = 0.0;
			out1 = 0.0;
		}
		__forceinline float64 process(float64 inputSample)noexcept override
		{
			const float64 output = b0 * inputSample + b1 * in1 - a1 * out1;
			in1 = inputSample;
			out1 = output;
			return output;
		}
	};

	class IIRFilter2ndOrder : public Effector
	{
		const float64 a1;
		const float64 a2;
		const float64 b0;
		const float64 b1;
		const float64 b2;
		float64 in1;
		float64 in2;
		float64 out1;
		float64 out2;

	public:
		IIRFilter2ndOrder(const iir::Coefficients2ndOrder &coefficients)noexcept :
			a1(coefficients.a1), a2(coefficients.a2), b0(coefficients.b0), b1(coefficients.b1), b2(coefficients.b2)
		{
			reset();
		}
		IIRFilter2ndOrder(float64 a1, float64 a2, float64 b0, float64 b1, float64 b2)noexcept :
			a1(a1), a2(a2), b0(b0), b1(b1), b2(b2)
		{
			reset();
		}
		IIRFilter2ndOrder(float64 a0, float64 a1, float64 a2, float64 b0, float64 b1, float64 b2)noexcept :
			a1(a1 / a0), a2(a2 / a0), b0(b0 / a0), b1(b1 / a0), b2(b2 / a0)
		{
			reset();
		}
		void reset()noexcept override
		{
			in1 = 0.0;
			in2 = 0.0;
			out1 = 0.0;
			out2 = 0.0;
		}
		__forceinline float64 process(float64 inputSample)noexcept override
		{
			const float64 output = b0 * inputSample + b1 * in1 + b2 * in2 - a1 * out1 - a2 * out2;
			in2 = in1;
			in1 = inputSample;
			out2 = out1;
			out1 = output;
			return output;
		}
	};

	class IIRFilterCascaded : public Effector
	{
		std::vector<IIRFilter1stOrder> iirFilters1stOrder;
		std::vector<IIRFilter2ndOrder> iirFilters2ndOrder;

	public:
		IIRFilterCascaded(const iir::CoefficientsCascaded &coefficients)
		{
			for(const iir::Coefficients1stOrder &filterCoefficients : coefficients.set1stOrder)
			{
				iirFilters1stOrder.emplace_back(filterCoefficients);
			}
			for(const iir::Coefficients2ndOrder &filterCoefficients : coefficients.set2ndOrder)
			{
				iirFilters2ndOrder.emplace_back(filterCoefficients);
			}
		}
		void reset()noexcept override
		{
			for(IIRFilter1stOrder &filter : iirFilters1stOrder)
			{
				filter.reset();
			}
			for(IIRFilter2ndOrder &filter : iirFilters2ndOrder)
			{
				filter.reset();
			}
		}
		__forceinline float64 process(float64 inputSample)noexcept override
		{
			float64 ret = inputSample;
			for(IIRFilter1stOrder &filter : iirFilters1stOrder)
			{
				ret = filter(ret);
			}
			for(IIRFilter2ndOrder &filter : iirFilters2ndOrder)
			{
				ret = filter(ret);
			}
			return ret;
		}
	};
	/*
	class IIRFilter : public Effector
	{
		iir::Coefficients coefficients;
		RingBuffer in;
		RingBuffer out;

		void setCoefficients(const iir::Coefficients &coefficients)
		{
			this->coefficients = coefficients.normalize();
		}

	public:
		IIRFilter(const iir::Coefficients &coefficients) :
			in(coefficients.b.getOrder() + 1LL),
			out(coefficients.a.getOrder() + 1LL)
		{
			setCoefficients(coefficients);
			reset();
		}
		IIRFilter &operator=(const iir::Coefficients &coefficients)
		{
			setCoefficients(coefficients);
			return *this;
		}
		void reset()override
		{
			in.clear();
			out.clear();
		}
		__forceinline float64 process(float64 inputSample)override
		{
			in.push(inputSample);
			out.push(0.0);
			for(uint32_t n = 0; n < in.getNumSamples(); ++n)
			{
				out[out.getNumSamples() - 1] += coefficients.b[n] * in[in.getNumSamples() - n - 1];
			}
			for(uint32_t n = 1; n < out.getNumSamples(); ++n)
			{
				out[out.getNumSamples() - 1] -= coefficients.a[n] * out[out.getNumSamples() - n - 1];
			}
			return out[0];
		}
		Audio processAutomation(const Audio &inputSignal, const std::vector<iir::Coefficients> automation)
		{
			Audio outputSignal(inputSignal.getFirstTime(), inputSignal.getLastTime());
			for(int n = inputSignal.getFirstTime(); n < inputSignal.getLastTime(); ++n)
			{
				setCoefficients(automation[n]);
				outputSignal[n] = process(inputSignal[n]);
			}
			return outputSignal;
		}
	};*/

	class IIRFilter1stOrderEx : public Effector
	{
		const float64 a1;
		const float64 b0;
		const float64 b1;
		float64 in1;
		float64 out1;
		Effector &effector;

	public:
		IIRFilter1stOrderEx(const iir::Coefficients1stOrder &coefficients)noexcept :
			a1(coefficients.a1), b0(coefficients.b0), b1(coefficients.b1),
			effector(effector)
		{
			reset();
		}
		IIRFilter1stOrderEx(float64 a1, float64 b0, float64 b1)noexcept :
			a1(a1), b0(b0), b1(b1),
			effector(effector)
		{
			reset();
		}
		IIRFilter1stOrderEx(float64 a0, float64 a1, float64 b0, float64 b1)noexcept :
			a1(a1 / a0), b0(b0 / a0), b1(b1 / a0),
			effector(effector)
		{
			reset();
		}
		void reset()noexcept override
		{
			in1 = 0.0;
			out1 = 0.0;
			effector.reset();
		}
		__forceinline float64 process(float64 inputSample)noexcept override
		{
			const float64 output = effector(b0 * inputSample + b1 * in1 - a1 * out1);
			in1 = inputSample;
			out1 = output;
			return output;
		}
	};

	class IIRFilter2ndOrderEx : public Effector
	{
		const float64 a1;
		const float64 a2;
		const float64 b0;
		const float64 b1;
		const float64 b2;
		float64 in1;
		float64 in2;
		float64 out1;
		float64 out2;
		Effector &effector;

	public:
		IIRFilter2ndOrderEx(const iir::Coefficients2ndOrder &coefficients, Effector &effector)noexcept :
			a1(coefficients.a1), a2(coefficients.a2), b0(coefficients.b0), b1(coefficients.b1), b2(coefficients.b2),
			effector(effector)
		{
			reset();
		}
		IIRFilter2ndOrderEx(float64 a1, float64 a2, float64 b0, float64 b1, float64 b2, Effector &effector)noexcept :
			a1(a1), a2(a2), b0(b0), b1(b1), b2(b2),
			effector(effector)
		{
			reset();
		}
		IIRFilter2ndOrderEx(float64 a0, float64 a1, float64 a2, float64 b0, float64 b1, float64 b2, Effector &effector)noexcept :
			a1(a1 / a0), a2(a2 / a0), b0(b0 / a0), b1(b1 / a0), b2(b2 / a0),
			effector(effector)
		{
			reset();
		}
		void reset()noexcept override
		{
			in1 = 0.0;
			in2 = 0.0;
			out1 = 0.0;
			out2 = 0.0;
			effector.reset();
		}
		__forceinline float64 process(float64 inputSample)noexcept override
		{
			const float64 output = effector(b0 * inputSample + b1 * in1 + b2 * in2 - a1 * out1 - a2 * out2);
			in2 = in1;
			in1 = inputSample;
			out2 = out1;
			out1 = output;
			return output;
		}
	};
}