export module sense.sinc_interpolation;

export import subspace.time;
import subspace.discrete_time;
import subspace.value_type;
import deeproot;
import sense.linear_interpolation;

namespace sense
{
	constexpr unsigned interpolationQuality = 10;
	constexpr subspace::value_t epsilon = 1.0e-7;

	export template <class Signal>
	constexpr auto estimateBySincInterpolation(const Signal &signal, subspace::Time t, int quality = interpolationQuality)noexcept
	{
		if(quality <= 1)
		{
			return estimateByLinearInterpolation(signal, t);
		}
		const subspace::discrete_time l = static_cast<subspace::discrete_time>(deeproot::floor(t));
		const subspace::discrete_time r = l + 1LL;
		if(t - l <= epsilon)
		{
			return signal.at(l);
		}
		else if(r - t <= epsilon)
		{
			return signal.at(r);
		}
		auto &&ret = signal.at(l) * 0.0;
		const subspace::value_t a = deeproot::sin(deeproot::pi * (t - l));
		for(subspace::discrete_time i = -quality; i < quality; ++i)
		{
			const subspace::discrete_time n = r + i;
			const subspace::value_t piDelta = deeproot::pi * (t - n);
			ret += signal.at(n) * (deeproot::hann(piDelta / quality) * deeproot::powerOfNegative1(i) * a / piDelta);
		}
		return ret;
	}

	export template <class Signal>
	constexpr auto estimateBySincInterpolationDownsampling(const Signal &signal, subspace::Time t, subspace::value_t sampleRateRatio, int quality = interpolationQuality)noexcept
	{
		if(quality <= 1)
		{
			return estimateByLinearInterpolation(signal, t);
		}
		if(sampleRateRatio > 1.0)
		{
			return estimateBySincInterpolation(signal, t);
		}
		if(quality < 1.0 / sampleRateRatio)
		{
			quality = static_cast<unsigned>(1.0 / sampleRateRatio) + 1;
		}
		const subspace::discrete_time l = static_cast<subspace::discrete_time>(deeproot::floor(t));
		const subspace::discrete_time r = l + 1;
		auto &&ret = (t - l <= epsilon) ? signal.at(l) : (r - t <= epsilon) ? signal.at(r) : 0.0;
		for(subspace::discrete_time i = -quality; i < quality; ++i)
		{
			const subspace::discrete_time n = r + i;
			const subspace::value_t piDelta = deeproot::pi * (t - n);
			ret += signal.at(n) * (deeproot::hann(piDelta / quality) * sampleRateRatio * deeproot::sinc(sampleRateRatio * piDelta));
		}
		return ret;
	}
}