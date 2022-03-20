module;

#include <vector>

export module sense.riff_chunk_assembler;

export import sense.riff_chunk;

namespace sense
{
	constexpr int numBitsPerByte = 8;

	template <class Sample>
	constexpr FmtChunk assembleFormatChunk(uint16_t numChannels, uint32_t sampleRate)
	{
		FmtChunk ret;
		ret.chunk_ID = ' tmf';
		ret.chunk_size = 16;
		ret.wave_format_type = getWaveFormatType<Sample>();
		ret.channel = numChannels;
		ret.samples_per_sec = sampleRate;
		ret.bits_per_sample = sizeof(Sample) * numBitsPerByte;
		ret.block_size = ret.bits_per_sample * numChannels / numBitsPerByte;
		ret.bytes_per_sec = ret.block_size * sampleRate;
		return ret;
	}

	DataChunk assembleDataChunk(uint16_t numChannels, uint32_t numSamples, size_t sampleSize, const std::vector<uint8_t> &signalData)
	{
		DataChunk ret;
		ret.chunk_ID = 'atad';
		ret.chunk_size = static_cast<uint32_t>(numChannels * numSamples * sampleSize);
		ret.signalData = signalData;
		return ret;
	}

	DataChunk assembleDataChunk(uint16_t numChannels, uint32_t numSamples, size_t sampleSize, std::vector<uint8_t> &&signalData)
	{
		DataChunk ret;
		ret.chunk_ID = 'atad';
		ret.chunk_size = static_cast<uint32_t>(numChannels * numSamples * sampleSize);
		ret.signalData = std::move(signalData);
		return ret;
	}

	export template <class Sample>
	RIFFChunk assembleRIFFChunk(uint16_t numChannels, uint32_t sampleRate, uint32_t numSamples, const std::vector<uint8_t> &data)
	{
		RIFFChunk ret;
		ret.chunk_ID = 'FFIR';
		ret.chunk_size = 36 + numChannels * numSamples * sizeof(Sample);
		ret.format_type = 'EVAW';
		ret.fmt_chunk = assembleFormatChunk<Sample>(numChannels, sampleRate);
		ret.data_chunk = assembleDataChunk(numChannels, numSamples, sizeof(Sample), data);
		return ret;
	}

	export template <class Sample>
	RIFFChunk assembleRIFFChunk(uint16_t numChannels, uint32_t sampleRate, uint32_t numSamples, const std::vector<uint8_t> &&data)
	{
		RIFFChunk ret;
		ret.chunk_ID = 'FFIR';
		ret.chunk_size = 36 + numChannels * numSamples * sizeof(Sample);
		ret.format_type = 'EVAW';
		ret.fmt_chunk = assembleFormatChunk<Sample>(numChannels, sampleRate);
		ret.data_chunk = assembleDataChunk(numChannels, numSamples, sizeof(Sample), std::move(data));
		return ret;
	}
}