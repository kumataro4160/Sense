module;

#include <vector>

export module sense.ifft;

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
	inline std::vector<deeproot::complex<subspace::value_t>> ifftCalculate(std::vector<deeproot::complex<subspace::value_t>>& x, std::vector<deeproot::complex<subspace::value_t>>& y, int n)
	{
		int strideWidth = 1;

		for(int nHalf = n / 2; nHalf >= 1; nHalf /= 2)
		{
			const subspace::value_t theta0 = deeproot::pi / nHalf;
			int i, j;
#pragma omp for private(i, j)
			for(i = 0; i < nHalf; ++i)
			{
				const subspace::value_t theta = i * theta0;
				const deeproot::complex<subspace::value_t> w(cos(theta), sin(theta));
				for(j = 0; j < strideWidth; ++j)
				{
					y[strideWidth * i * 2 + j] = x[strideWidth * i + j] + x[strideWidth * (i + nHalf) + j];
					y[strideWidth * (i * 2 + 1) + j] = (x[strideWidth * i + j] - x[strideWidth * (i + nHalf) + j]) * w;
				}
			}
			strideWidth *= 2;
			std::swap(x, y);
		}
		return x;
	}

	inline std::vector<subspace::Stereo<deeproot::complex<subspace::value_t>>> ifftCalculate(std::vector<subspace::Stereo<deeproot::complex<subspace::value_t>>>& x, std::vector<subspace::Stereo<deeproot::complex<subspace::value_t>>>& y, int n)
	{
		int strideWidth = 1;

		for(int nHalf = n / 2; nHalf >= 1; nHalf /= 2)
		{
			const subspace::value_t theta0 = deeproot::pi / nHalf;
			int i, j;
#pragma omp for private(i, j)
			for(i = 0; i < nHalf; ++i)
			{
				const subspace::value_t theta = i * theta0;
				const deeproot::complex<subspace::value_t> w(cos(theta), sin(theta));
				for(j = 0; j < strideWidth; ++j)
				{
					y[strideWidth * i * 2 + j] = x[strideWidth * i + j] + x[strideWidth * (i + nHalf) + j];
					y[strideWidth * (i * 2 + 1) + j] = (x[strideWidth * i + j] - x[strideWidth * (i + nHalf) + j]) * w;
				}
			}
			strideWidth *= 2;
			std::swap(x, y);
		}
		return x;
	}

	export subspace::Waveform<deeproot::complex<subspace::value_t>> ifft(const subspace::Spectrum<deeproot::complex<subspace::value_t>>& spectrum)
	{
		const subspace::discrete_time length = spectrum.getLength();
		if(length == 0 || length == 1 || !deeproot::isPowerOf2(length))
		{
			throw L"ifft : ’è‹`‚³‚ê‚Ä‚¢‚È‚¢“ü—Í";
		}
		std::vector<deeproot::complex<subspace::value_t>> x(spectrum.begin(), spectrum.end());
		std::vector<deeproot::complex<subspace::value_t>> y(length);
		return ifftCalculate(x, y, length);
	}

	export subspace::Waveform<subspace::Stereo<deeproot::complex<subspace::value_t>>> ifft(const subspace::Spectrum<subspace::Stereo<deeproot::complex<subspace::value_t>>>& spectrum)
	{
		const subspace::discrete_time length = spectrum.getLength();
		if(length == 0 || length == 1 || !deeproot::isPowerOf2(length))
		{
			throw L"ifft : ’è‹`‚³‚ê‚Ä‚¢‚È‚¢“ü—Í";
		}
		std::vector<subspace::Stereo<deeproot::complex<subspace::value_t>>> x(spectrum.begin(), spectrum.end());
		std::vector<subspace::Stereo<deeproot::complex<subspace::value_t>>> y(length);
		return ifftCalculate(x, y, length);
	}
}