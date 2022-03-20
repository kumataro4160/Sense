module;

#include <string>

export module sense.waveform_exporter;

export import subspace.waveform;
import sense.signal_serializer;
import sense.riff_chunk_assembler;
import sense.riff_chunk_serializer;
import sense.binary;
import subspace.value_type;
import subspace.stereo;
import subspace.ms;
import subspace.multi;

namespace sense
{
	template <class Frame>
	uint16_t getNumSamples()
	{
		throw "exportWaveformAsWavFloat64 : FrameÇÃéwíËÇ™ñ≥å¯Ç≈Ç∑ÅB";
	}

	template <>
	uint16_t getNumSamples<subspace::value_t>()
	{
		return 1;
	}

	template <>
	uint16_t getNumSamples<subspace::Stereo<subspace::value_t>>()
	{
		return 2;
	}

	template <>
	uint16_t getNumSamples<subspace::MS<subspace::value_t>>()
	{
		return 2;
	}

	export template <class Frame>
	void exportWavFloat64(const subspace::Waveform<Frame> &waveform, uint32_t sampleRate, std::string fileName)
	{
		std::vector<uint8_t> signalData = serializeWaveform(waveform);
		const RIFFChunk &riffChunk = assembleRIFFChunk<subspace::value_t>(getNumSamples<Frame>(), sampleRate, static_cast<uint32_t>(waveform.getLength()), std::move(signalData));
		const Binary &binary = serializeRIFFChunk(riffChunk);
		binary.write(fileName);
	}

	export template <size_t N>
	void exportWavFloat64(const subspace::Waveform<subspace::Multi<subspace::value_t, N>> &waveform, uint32_t sampleRate, std::string fileName)
	{
		std::vector<uint8_t> signalData = serializeWaveform(waveform);
		const RIFFChunk &riffChunk = assembleRIFFChunk<subspace::value_t>(N, sampleRate, waveform.getLength(), std::move(signalData));
		const Binary &binary = serializeRIFFChunk(riffChunk);
		binary.write(fileName);
	}
}