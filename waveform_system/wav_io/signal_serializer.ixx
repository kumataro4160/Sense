module;

#include <vector>

export module sense.signal_serializer;
import subspace.value_type;
import subspace.stereo;
import subspace.ms;
import subspace.multi;
import subspace.waveform;
import subspace.spectrum;

namespace sense
{
	template <class Signal>
	void serializeFrame(const Signal &signal, const std::vector<uint8_t>::iterator &signalData)
	{
		throw "serializeFrame : éwíËÇ≥ÇÍÇΩSignalÇ…ëŒâûÇµÇƒÇ¢Ç‹ÇπÇÒÅB";
	}

	template <>
	void serializeFrame(const subspace::value_t &sample, const std::vector<uint8_t>::iterator &signalData)
	{
		const size_t size = sizeof(subspace::value_t);
		union
		{
			subspace::value_t floatType;
			uint8_t binaryType[size];
		};
		floatType = sample;
		for(size_t i = 0; i < size; ++i)
		{
			signalData[i] = binaryType[i];
		}
	}

	template <>
	void serializeFrame(const subspace::Stereo<subspace::value_t> &stereoFrame, const std::vector<uint8_t>::iterator &signalData)
	{
		serializeFrame(stereoFrame.l, signalData);
		serializeFrame(stereoFrame.r, signalData + sizeof(subspace::value_t));
	}

	template <>
	void serializeFrame(const subspace::MS<subspace::value_t> &msFrame, const std::vector<uint8_t>::iterator &signalData)
	{
		serializeFrame(msFrame.m, signalData);
		serializeFrame(msFrame.s, signalData + sizeof(subspace::value_t));
	}

	template <size_t N>
	void serializeFrame(const subspace::Multi<subspace::value_t, N> &multiFrame, const std::vector<uint8_t>::iterator &signalData)
	{
		for(size_t i = 0; i < N; ++i)
		{
			serializeFrame(multiFrame[i], signalData + i * sizeof(subspace::value_t));
		}
	}

	export template <class Frame>
	std::vector<uint8_t> serializeWaveform(const subspace::Waveform<Frame> &waveform)
	{
		const size_t length = waveform.getLength();
		std::vector<uint8_t> ret(length * sizeof(Frame));
		for(size_t i = 0; i < length; ++i)
		{
			serializeFrame(waveform[i], ret.begin() + i * sizeof(Frame));
		}
		return ret;
	}

	export template <class Frame>
	std::vector<uint8_t> serializeSpectrum(const subspace::Spectrum<Frame> &spectrum)
	{
		const size_t length = spectrum.getLength();
		std::vector<uint8_t> ret(length * sizeof(Frame));
		for(size_t i = 0; i < length; ++i)
		{
			serializeFrame(spectrum[i], ret.begin() + i * sizeof(Frame));
		}
		return ret;
	}
}