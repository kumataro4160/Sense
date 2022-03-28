#pragma once

#include "wave_table_1d.hpp"


namespace sense
{
	WaveTable1D createSineWaveTable()
	{
		WaveTable1D ret;

		for(uint n = 0; n <= tableSize / 4; ++n)
		{
			ret[n] = sin(2.0 * pi * n / tableSize);
		}
		for(uint n = tableSize / 4 + 1; n < tableSize / 2; ++n)
		{
			ret[n] = ret[tableSize / 2 - n];
		}
		for(uint n = tableSize / 2; n < tableSize; ++n)
		{
			ret[n] = -ret[n - tableSize / 2];
		}

		return ret;
	}

	WaveTable1D createSawtoothWaveTable()
	{
		WaveTable1D ret;

		for(uint n = 0; n < tableSize; ++n)
		{
			ret[n] = -1.0 + (2.0 * n / (tableSize - 1));
		}

		return ret;
	}

	WaveTable1D createSquareWaveTable()
	{
		WaveTable1D ret;

		for(uint n = 0; n < tableSize / 2; ++n)
		{
			ret[n] = 1.0;
		}
		for(uint n = tableSize / 2; n < tableSize; ++n)
		{
			ret[n] = -1.0;
		}

		return ret;
	}

	WaveTable1D createTriangleWaveTable()
	{
		WaveTable1D ret;

		for(uint n = 0; n <= tableSize / 4; ++n)
		{
			ret[n] = 4.0 * n / tableSize;
		}
		for(uint n = tableSize / 4 + 1; n < tableSize / 2; ++n)
		{
			ret[n] = ret[tableSize / 2 - n];
		}
		for(uint n = tableSize / 2; n < tableSize; ++n)
		{
			ret[n] = -ret[n - tableSize / 2];
		}

		return ret;
	}
}