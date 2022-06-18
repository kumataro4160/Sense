export module sense.pythagorean_tuning;

import sense.equal_temperament;

export namespace sense
{
	constexpr double calc_F_PythagoreanTuning(int transposeFromC3, int basedToneFromC3, double F_basedTone)
	{
		/*auto cycle = [](int x, int y)->int
		{
			return x >= 0 ? x % y : (x + 1) / y - 1;
		};*/
		const double ratio[] = {1.0, 256.0 / 243.0, 9.0 / 8.0, 32.0 / 27.0, 81.0 / 64.0, 4.0 / 3.0, 729.0 / 512.0, 3.0 / 2.0, 128.0 / 81.0, 27.0 / 16.0, 16.0 / 9.0, 243.0 / 128.0};
		/*for(int i = -5; i <= 6; ++i)
		{
			ratio[i*7]=gcem::pow(1.5, i);
		}*/
		const int transposeFromBasedTone = transposeFromC3 - basedToneFromC3;
		const int oct = transposeFromBasedTone >= 0 ? transposeFromBasedTone / 12 : (transposeFromBasedTone + 1) / 12 - 1;
		const int subTranspose = transposeFromBasedTone - 12 * oct;
		return F_basedTone * oct * ratio[subTranspose];
	}

	constexpr double calc_F_PythagoreanTuning(int transposeFromC3)
	{
		return calc_F_PythagoreanTuning(transposeFromC3, 0, calc_F_EqualTemperament(0));
	}
}