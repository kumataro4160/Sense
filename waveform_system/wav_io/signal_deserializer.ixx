module;

#include <vector>

export module sense.signal_deserializer;
import subspace.value_type;
import subspace.stereo;
import subspace.ms;
import subspace.multi;
import subspace.waveform;
import subspace.spectrum;

namespace sense
{
	subspace::value_t deserializeSample(const std::vector<uint8_t>::const_iterator &signalData)
	{
		return *(subspace::value_t *)(&signalData[0]);
	}

	template <class Frame>
	Frame deserializeFrame(const std::vector<uint8_t>::const_iterator &signalData)
	{
		throw "deserializeFrame : éwíËÇ≥ÇÍÇΩSignalÇ…ëŒâûÇµÇƒÇ¢Ç‹ÇπÇÒÅB";
	}

	template <>
	subspace::value_t deserializeFrame(const std::vector<uint8_t>::const_iterator &signalData)
	{
		return deserializeSample(signalData);
	}

	template <>
	subspace::Stereo<subspace::value_t> deserializeFrame(const std::vector<uint8_t>::const_iterator &signalData)
	{
		subspace::Stereo<subspace::value_t> ret;
		ret.l = deserializeSample(signalData);
		ret.r = deserializeSample(signalData + sizeof(subspace::value_t));
		return ret;
	}

	template <>
	subspace::MS<subspace::value_t> deserializeFrame(const std::vector<uint8_t>::const_iterator &signalData)
	{
		subspace::MS<subspace::value_t> ret;
		ret.m = deserializeSample(signalData);
		ret.s = deserializeSample(signalData + sizeof(subspace::value_t));
		return ret;
	}

	template <size_t N>
	subspace::Multi<subspace::value_t, N> deserializeFrame(const std::vector<uint8_t>::const_iterator &signalData)
	{
		subspace::Multi<subspace::value_t, N> ret;
		for(size_t i = 0; i < N; ++i)
		{
			ret[i] = deserializeSample(signalData + i * sizeof(subspace::value_t));
		}
		return ret;
	}

	export template <class Frame>
	subspace::Waveform<Frame> deserializeWaveform(const std::vector<uint8_t> &signalData)
	{
		const size_t length = signalData.size() / sizeof(Frame);
		subspace::Waveform<Frame> ret(length);
		for(size_t i = 0; i < length; ++i)
		{
			ret[i] = deserializeFrame<Frame>(signalData.begin() + i * sizeof(Frame));
		}
		return ret;
	}

	export template <class Frame>
	subspace::Spectrum<Frame> deserializeSpectrum(const std::vector<uint8_t> &signalData)
	{
		const size_t length = signalData.size() / sizeof(Frame);
		subspace::Spectrum<Frame> ret(length);
		for(size_t i = 0; i < length; ++i)
		{
			ret[i] = deserializeFrame<Frame>(signalData.begin() + i * sizeof(Frame));
		}
		return ret;
	}
}