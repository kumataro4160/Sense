export module sense.just_intonation;

import sense.equal_temperament;

export namespace sense
{
	constexpr double calc_F_JustIntonation(int transposeFromC3, int basedToneFromC3, double F_basedTone)
	{
		constexpr double ratio[] = {1.0, 16.0 / 15.0, 9.0 / 8.0, 6.0 / 5.0, 5.0 / 4.0, 4.0 / 3.0, 45.0 / 32.0, 3.0 / 2.0, 8.0 / 5.0, 5.0 / 3.0, 9.0 / 5.0, 15.0 / 8.0};
		const int transposeFromBasedTone = transposeFromC3 - basedToneFromC3;
		const int oct = transposeFromBasedTone >= 0 ? transposeFromBasedTone / 12 : (transposeFromBasedTone + 1) / 12 - 1;
		const int subTranspose = transposeFromBasedTone - 12 * oct;
		return F_basedTone * oct * ratio[subTranspose];
	}

	constexpr double calc_F_JustIntonation(int transposeFromC3)
	{
		return calc_F_JustIntonation(transposeFromC3, 0, calc_F_EqualTemperament(0));
	}
}