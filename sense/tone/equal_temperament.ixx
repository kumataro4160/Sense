export module sense.equal_temperament;

import deeproot;

export namespace sense
{
	constexpr double calc_F_EqualTemperament(int transposeFromC3, int basedToneFromC3, double F_basedTone)
	{
		return F_basedTone * deeproot::pow(2.0, (transposeFromC3 - basedToneFromC3) / 12.0);
	}

	constexpr double calc_F_EqualTemperament(int transposeFromC3)
	{
		return calc_F_EqualTemperament(transposeFromC3, 9, 440.0);
	}
}