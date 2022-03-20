module;

#include <vector>

export module sense.fft;

export import deeproot.complex;
export import subspace.waveform;
export import subspace.spectrum;
export import subspace.stereo;
import deeproot.pi;
import deeproot.ispowerof2;
import deeproot.sin;
import deeproot.cos;
import subspace.value_type;

namespace multiverse
{
	inline std::vector<deeproot::complex<subspace::value_t>> fftCalculate(std::vector<deeproot::complex<subspace::value_t>> &x, std::vector<deeproot::complex<subspace::value_t>> &y, int n)
	{
		int strideWidth = 1;

		for(int nHalf = n / 2; nHalf >= 1; nHalf /= 2)
		{
			const subspace::value_t theta0 = -deeproot::pi / nHalf;
			int i, j;
#pragma omp for private(i, j)
			for(i = 0; i < nHalf; ++i)
			{
				const subspace::value_t theta = i * theta0;
				const deeproot::complex<subspace::value_t> w(cos(theta), sin(theta));
				for(j = 0; j < strideWidth; ++j)
				{
					y[strideWidth * i * 2 + j] = (x[strideWidth * i + j] + x[strideWidth * (i + nHalf) + j]) / 2.0;
					y[strideWidth * (i * 2 + 1) + j] = (x[strideWidth * i + j] - x[strideWidth * (i + nHalf) + j]) / 2.0 * w;
				}
			}
			strideWidth *= 2;
			std::swap(x, y);
		}
		return x;
	}

	inline std::vector<subspace::Stereo<deeproot::complex<subspace::value_t>>> fftCalculate(std::vector<subspace::Stereo<deeproot::complex<subspace::value_t>>> &x, std::vector<subspace::Stereo<deeproot::complex<subspace::value_t>>> &y, int n)
	{
		int strideWidth = 1;

		for(int nHalf = n / 2; nHalf >= 1; nHalf /= 2)
		{
			const subspace::value_t theta0 = -deeproot::pi / nHalf;
			int i, j;
#pragma omp for private(i, j)
			for(i = 0; i < nHalf; ++i)
			{
				const subspace::value_t theta = i * theta0;
				const deeproot::complex<subspace::value_t> w(cos(theta), sin(theta));
				for(j = 0; j < strideWidth; ++j)
				{
					y[strideWidth * i * 2 + j] = (x[strideWidth * i + j] + x[strideWidth * (i + nHalf) + j]) / deeproot::complex(2.0);
					y[strideWidth * (i * 2 + 1) + j] = (x[strideWidth * i + j] - x[strideWidth * (i + nHalf) + j]) / deeproot::complex(2.0) * w;
				}
			}
			strideWidth *= 2;
			std::swap(x, y);
		}
		return x;
	}

	export subspace::Spectrum<deeproot::complex<subspace::value_t>> fft(const subspace::Waveform<subspace::value_t> &waveform)
	{
		const subspace::discrete_time length = waveform.getLength();
		if(length == 0 || length == 1 || !deeproot::isPowerOf2(length))
		{
			throw L"fft : 定義されていない入力";
		}
		std::vector<deeproot::complex<subspace::value_t>> x(waveform.begin(), waveform.end());
		std::vector<deeproot::complex<subspace::value_t>> y(length);
		return fftCalculate(x, y, length);
	}

	export subspace::Spectrum<subspace::Stereo<deeproot::complex<subspace::value_t>>> fft(const subspace::Waveform<subspace::Stereo<subspace::value_t>> &waveform)
	{
		const subspace::discrete_time length = waveform.getLength();
		if(length == 0 || length == 1 || !deeproot::isPowerOf2(length))
		{
			throw L"fft : 定義されていない入力";
		}
		std::vector<subspace::Stereo<deeproot::complex<subspace::value_t>>> x(waveform.begin(), waveform.end());
		std::vector<subspace::Stereo<deeproot::complex<subspace::value_t>>> y(length);
		return fftCalculate(x, y, length);
	}
}

	/*
	template <class Frame>
	inline bool fft(std::vector<deeproot::complex<Frame>> input)
	{
		const size_t n = input.size();
		std::vector<deeproot::complex<Frame>> output(n);
		auto x = input.begin();
		auto y = output.begin();
		size_t strideWidth = 1;
		bool returnsInput = true;

		for(size_t nHalf = n / 2; nHalf >= 1; nHalf /= 2)
		{
			const subspace::value_t theta0 = -pi / nHalf;
			size_t i, j;
#pragma omp for private(i, j)
			for(i = 0; i < nHalf; ++i)
			{
				const subspace::value_t theta = i * theta0;
				const deeproot::complex<subspace::value_t> w(cos(theta), sin(theta));
				for(j = 0; j < strideWidth; ++j)
				{
					y[strideWidth * i * 2 + j] = (x[strideWidth * i + j] + x[strideWidth * (i + nHalf) + j]) / 2.0;
					y[strideWidth * (i * 2 + 1) + j] = (x[strideWidth * i + j] - x[strideWidth * (i + nHalf) + j]) / 2.0 * w;
				}
			}
			strideWidth *= 2;
			std::swap(x, y);
			returnsInput = !returnsInput;
		}
		return returnsInput;
	}
	*/