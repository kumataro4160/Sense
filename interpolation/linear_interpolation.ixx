export module sense.linear_interpolation;

export import subspace.time;
import subspace.discrete_time;
import deeproot.floor;

export namespace sense
{
	template <class Signal>
	constexpr auto estimateByLinearInterpolation(const Signal& signal, subspace::Time t)noexcept
	{
		const subspace::discrete_time l = static_cast<subspace::discrete_time>(deeproot::floor(t));
		const subspace::discrete_time r = l + 1LL;
		return signal.at(l) * (r - t) + signal.at(r) * (t - l);
	}
}