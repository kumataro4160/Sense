module;

#include <vector>

export module sense.riff_chunk_serializer;

export import sense.binary;
export import sense.riff_chunk;

namespace sense
{
	Binary serializeFormatChunk(const FmtChunk &chunk)
	{
		Binary ret;
		ret.read(&chunk, sizeof(FmtChunk));
		return ret;
	}

	Binary serializeDataChunk(const DataChunk &chunk)
	{
		Binary ret;
		ret.read(&chunk.chunk_ID, sizeof(uint32_t));
		ret.read(&chunk.chunk_size, sizeof(uint32_t));
		ret.read(&chunk.signalData[0], chunk.signalData.size());
		return ret;
	}

	export Binary serializeRIFFChunk(const RIFFChunk &chunk)
	{
		Binary ret;
		ret.read(&chunk.chunk_ID, sizeof(uint32_t));
		ret.read(&chunk.chunk_size, sizeof(uint32_t));
		ret.read(&chunk.format_type, sizeof(uint32_t));
		ret.read(serializeFormatChunk(chunk.fmt_chunk));
		ret.read(serializeDataChunk(chunk.data_chunk));
		return ret;
	}
}