module;

#include <tuple>
#include <vector>

export module sense.riff_chunk_disassembler;

export import sense.riff_chunk;

export namespace sense
{
	constexpr int numBitsPerByte = 8;

	std::tuple<uint16_t, uint32_t, uint32_t, std::vector<uint8_t>> disassembleRIFFChunk(const RIFFChunk &riffChunk)
	{
		const uint16_t numChannels = riffChunk.fmt_chunk.channel;
		const uint32_t sampleRate = riffChunk.fmt_chunk.samples_per_sec;
		const uint16_t sampleSize = riffChunk.fmt_chunk.bits_per_sample / numBitsPerByte;
		const uint32_t numSamples = riffChunk.data_chunk.chunk_size / (numChannels * sampleSize);
		const std::vector<uint8_t> signalData = riffChunk.data_chunk.signalData;
		return {numChannels, sampleRate, numSamples, signalData};
	}
}