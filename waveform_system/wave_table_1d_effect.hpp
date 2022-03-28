#pragma once

#include "wave_table_1d.hpp"


namespace sense
{
	std::vector<float64> makeRealSignal(const std::vector<cfloat64> &complexSignal)
	{
		std::vector<float64> ret(complexSignal.size());
#pragma omp for
		for(longlong i = 0; i < complexSignal.size(); ++i)
		{
			ret[i] = complexSignal[i].real();
		}
		return ret;
	}

	WaveTable1D removeFoldingNoise(float64 omega, const WaveTable1D &waveTable)
	{
		if(omega <= 2.0 * pi / tableSize)
		{
			return waveTable;
		}
		uint32_t max = static_cast<uint32_t>(pi / omega);
		spectrum::SpectrumRectangular &&spectrum = fft(waveTable.begin(), waveTable.end());
		for(uint32_t reducer = max; reducer <= tableSize - max; ++reducer)
		{
			spectrum[reducer] = 0.0;
		}
		return Loop<float64, tableSize>(makeRealSignal(ifft(spectrum.begin(), spectrum.end())));
	}

	WaveTable1D zeroAllPhase(const WaveTable1D &waveTable)
	{
		spectrum::SpectrumRectangular &&spectrum = fft(waveTable.begin(), waveTable.end());
		for(uint32_t reducer = 0; reducer < tableSize; ++reducer)
		{
			spectrum[reducer] = cfloat64(abs(spectrum[reducer]), 0.0);
		}
		return Loop<float64, tableSize>(makeRealSignal(ifft(spectrum.begin(), spectrum.end())));
	}

	WaveTable1D zeroFundamentalPhase(const WaveTable1D &waveTable)
	{
		spectrum::SpectrumRectangular &&spectrum = fft(waveTable.begin(), waveTable.end());
		spectrum[0] = cfloat64(abs(spectrum[0]), 0.0);
		spectrum[1] = cfloat64(abs(spectrum[1]), 0.0);
		spectrum[tableSize - 1] = cfloat64(abs(spectrum[tableSize - 1]), 0.0);
		return Loop<float64, tableSize>(makeRealSignal(ifft(spectrum.begin(), spectrum.end())));
	}

	WaveTable1D convolute(const WaveTable1D &waveTable1, const WaveTable1D &waveTable2)
	{
		const spectrum::SpectrumRectangular &spectrum12 = spectrum::SpectrumRectangular(fft(waveTable1.begin(), waveTable1.end())) * fft(waveTable2.begin(), waveTable2.end());
		return Loop<float64, tableSize>(makeRealSignal(ifft(spectrum12.begin(), spectrum12.end())));
	}
}