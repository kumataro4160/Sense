#pragma once

#include "../interpolation.hpp"
#include "../fir/operator.hpp"
#include <numeric>


namespace sense
{
	template <class signal_t>
	signal_t reverse(const signal_t &signal)
	{
		signal_t ret(signal);
		std::reverse(ret.begin(), ret.end());
		return ret;
	}

	template <class signal_t>
	signal_t reverse(signal_t &&signal)
	{
		std::reverse(signal.begin(), signal.end());
		return signal;
	}

	template <class signal_t>
	signal_t normalize(const signal_t &signal)
	{
		const std::pair &bottomAndTop = std::minmax_element(signal.begin(), signal.end());
		const float64 max = std::max(std::abs(*bottomAndTop.first), std::abs(*bottomAndTop.second));
		return signal / max;
	}

	template <class signal_t>
	signal_t normalize(signal_t &&signal)
	{
		const auto &bottomAndTop = std::minmax_element(signal.begin(), signal.end());
		const float64 max = std::max(std::abs(*bottomAndTop.first), std::abs(*bottomAndTop.second));
		return signal /= max;
	}

	template <class signal_t>
	signal_t removeDCOffset(const signal_t &signal)
	{
		const float64 mean = std::reduce(signal.begin(), signal.end(), 0.0) / signal.getLength();
		return signal - mean;
	}

	template <class signal_t>
	signal_t removeDCOffset(signal_t &&signal)
	{
		const float64 mean = std::reduce(signal.begin(), signal.end(), 0.0) / signal.getLength();
		return signal -= mean;
	}

	template <class signal_t>
	signal_t delay(const signal_t &signal, int t)
	{
		const int startTime = signal.getFirstTime();
		const int endTime = signal.getLastTime();
		signal_t ret(startTime, endTime);
#pragma omp for
		for(int i = startTime; i < endTime; ++i)
		{
			ret[i] = signal.at(i - t);
		}
		return ret;
	}

	template <class signal_t>
	signal_t delay(const signal_t &signal, float64 t, uint quality = interpolationQuality)
	{
		const int startTime = signal.getFirstTime();
		const int endTime = signal.getLastTime();
		signal_t ret(startTime, endTime);
#pragma omp for
		for(int i = startTime; i < endTime; ++i)
		{
			ret[i] = interpolateSinc(signal, i - t, quality);
		}
		return ret;
	}

	template <class signal_t>
	signal_t convolute(const signal_t &audio1, const signal_t &audio2)
	{
		signal_t audioTmp1(audio1);
		signal_t audioTmp2(audio2);
		const int audio1FirstTime = audio1.getFirstTime();
		const int audio2FirstTime = audio2.getFirstTime();
		audioTmp1 <<= audio1FirstTime;
		audioTmp2 <<= audio2FirstTime;
		const int resultSize = audio1.getLength() + audio2.getLength() - 1;
		const int fftSize = nearPowOf2(resultSize);
		audioTmp1.resize(0, fftSize);
		audioTmp2.resize(0, fftSize);
		const spectrum::SpectrumRectangular &spectrum12 = fft(audioTmp1) * fft(audioTmp2) * (1.0 * fftSize);
		signal_t ret(ifft(spectrum12, 0).resize(0, resultSize));
		ret >>= (audio1FirstTime + audio2FirstTime);
		return ret;
	}

	template <class signal_t>
	signal_t convolute(signal_t &&audio1, signal_t &&audio2)
	{
		const int audio1FirstTime = audio1.getFirstTime();
		const int audio2FirstTime = audio2.getFirstTime();
		audio1 <<= audio1FirstTime;
		audio2 <<= audio2FirstTime;
		const int resultSize = audio1.getLength() + audio2.getLength() - 1;
		const int fftSize = nearPowOf2(resultSize);
		audio1.resize(0, fftSize);
		audio2.resize(0, fftSize);
		const spectrum::SpectrumRectangular &spectrum12 = fft(audio1) * fft(audio2) * (1.0 * fftSize);
		audio1.resize(0, 0);
		audio2.resize(0, 0);
		signal_t ret(ifft(spectrum12, 0).resize(0, resultSize));
		ret >>= (audio1FirstTime + audio2FirstTime);
		return ret;
	}

	template <class signal_t>
	signal_t deconvolute(const signal_t &audio1, const signal_t &audio2)
	{
		signal_t audioTmp1(audio1);
		signal_t audioTmp2(audio2);
		const int audio1FirstTime = audio1.getFirstTime();
		const int audio2FirstTime = audio2.getFirstTime();
		audioTmp1 <<= audio1FirstTime;
		audioTmp2 <<= audio2FirstTime;
		const int resultSize = audio1.getLength() + audio2.getLength() - 1;
		const int fftSize = nearPowOf2(resultSize);
		audioTmp1.resize(0, fftSize);
		audioTmp2.resize(0, fftSize);
		const spectrum::SpectrumRectangular &spectrum12 = fft(audioTmp1) / (fft(audioTmp2) * (1.0 * fftSize));
		signal_t ret(ifft(spectrum12, 0).resize(0, resultSize));
		ret >>= (audio1FirstTime + audio2FirstTime);
		return ret;
	}

	template <class signal_t>
	signal_t deconvolute(signal_t &&audio1, signal_t &&audio2)
	{
		const int audio1FirstTime = audio1.getFirstTime();
		const int audio2FirstTime = audio2.getFirstTime();
		audio1 <<= audio1FirstTime;
		audio2 <<= audio2FirstTime;
		const int resultSize = audio1.getLength() + audio2.getLength() - 1;
		const int fftSize = nearPowOf2(resultSize);
		audio1.resize(0, fftSize);
		audio2.resize(0, fftSize);
		const spectrum::SpectrumRectangular &spectrum12 = fft(audio1) / (fft(audio2) * (1.0 * fftSize));
		audio1.resize(0, 0);
		audio2.resize(0, 0);
		signal_t ret(ifft(spectrum12, 0).resize(0, resultSize));
		ret >>= (audio1FirstTime + audio2FirstTime);
		return ret;
	}

	template <class signal_t>
	signal_t computeAutocorrelationFunction(const signal_t &audio)
	{
		const int audioLength = audio.getLength();
		const int fftSize = nearPowOf2(2 * audioLength);
		signal_t signal(0, fftSize);
		signal += audio << audio.getFirstTime();
		spectrum::SpectrumRectangular &&spectrum = fft(signal);
#pragma omp for
		for(int i = 0; i < fftSize; ++i)
		{
			spectrum[i] = squared(spectrum[i].real()) + squared(spectrum[i].imag());
		}
		return ifft(spectrum, 0).resize(0, audioLength) *= (1.0 * fftSize);
	}

	template <class signal_t>
	signal_t computeAutocorrelationFunction(signal_t &&audio)
	{
		const int audioLength = audio.getLength();
		const int fftSize = nearPowOf2(2 * audioLength);
		audio <<= audio.getFirstTime();
		audio.resize(0, fftSize);
		spectrum::SpectrumRectangular &&spectrum = fft(audio);
#pragma omp for
		for(int i = 0; i < fftSize; ++i)
		{
			spectrum[i] = squared(spectrum[i].real()) + squared(spectrum[i].imag());
		}
		return ifft(spectrum, 0).resize(0, audioLength) *= (1.0 * fftSize);
	}

	template <class signal_t>
	signal_t stretchAudio(const signal_t &audio, float64 frequencyRatio)
	{
		const int nextFirstTime = static_cast<int>(std::floor((audio.getFirstTime() - 1) / frequencyRatio)) + 1;
		const int nextLastTime = static_cast<int>(std::ceil(audio.getLastTime() / frequencyRatio));
		signal_t ret(nextFirstTime, nextLastTime);
#pragma omp for
		for(int i = nextFirstTime; i < nextLastTime; ++i)
		{
			ret[i] = interpolateSincDownsampling(audio, i * frequencyRatio, 1.0 / frequencyRatio);
		}
		return ret;
	}

	template <class signal_t>
	signal_t resampleAudio(const signal_t &audio, float64 sampleRateRatio)
	{
		return stretchAudio(audio, 1.0 / sampleRateRatio);
	}

	template <class signal_t>
	signal_t shiftPitch(const signal_t &audio, float64 frequencyRatio, const fir::Operator &window)
	{
		signal_t ret(audio.getFirstTime(), audio.getLastTime());
		const int cycle = window.getLength() / 2;
		for(int i = 0; i <= audio.getLength() / cycle; ++i)
		{
			const int shift = -audio.getFirstTime() + cycle * i;
			const signal_t &piece = (audio * (window >> shift)) << shift;
			ret += stretchAudio(piece, frequencyRatio) >> shift;
		}
		ret.resize(audio.getFirstTime(), audio.getLastTime());
		return ret;
	}

	template <class signal_t>
	signal_t pruneAudio(const signal_t &audio, const fir::Operator &window, float64 time, uint quality = interpolationQuality)
	{
		const int startTime = window.getFirstTime();
		const int endTime = window.getLastTime();
		signal_t ret(startTime, endTime);
#pragma omp for
		for(int i = startTime; i < endTime; ++i)
		{
			ret[i] = window[i] * interpolateSinc(audio, i + time, quality);
		}
		return ret;
	}

	template <class signal_t>
	signal_t timestretchAudio(const signal_t &audio, float64 timeRatio, const fir::Operator &window, uint quality = interpolationQuality)
	{
		const int nextFirstTime = static_cast<int>(std::floor((audio.getFirstTime() - 1) * timeRatio)) + 1;
		const int nextLastTime = static_cast<int>(std::ceil(audio.getLastTime()) * timeRatio);
		signal_t ret(nextFirstTime, nextLastTime);
		const int cycle = window.getLength() / 2;
		for(int i = 0; i <= audio.getLength() / cycle; ++i)
		{
			const float64 shiftL = -audio.getFirstTime() + cycle / timeRatio * i;
			const int shiftR = -audio.getFirstTime() + cycle * i;
			ret += pruneAudio(audio, window, shiftL, quality) >> shiftR;
		}
		ret.resize(nextFirstTime, nextLastTime);
		return ret;
	}
}