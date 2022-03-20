module;

#include <tuple>
#include <vector>
#include <string>

export module sense.waveform_importer;

export import subspace.waveform;
import sense.binary;
import sense.riff_chunk_deserializer;
import sense.riff_chunk_disassembler;
import sense.signal_deserializer;
import subspace.value_type;
import subspace.stereo;
import subspace.ms;
import subspace.multi;

export namespace sense
{
	template <class Frame>
	std::tuple<subspace::Waveform<Frame>, uint32_t> importWavFloat64(std::string fileName)
	{
		Binary binary(fileName);
		const RIFFChunk &riffChunk = deserializeRIFFChunk(binary);
		auto [numChannels, sampleRate, numSamples, signalData] = disassembleRIFFChunk(riffChunk);
		const subspace::Waveform<Frame> &waveform = deserializeWaveform<Frame>(signalData);
		return {waveform, sampleRate};
	}
}