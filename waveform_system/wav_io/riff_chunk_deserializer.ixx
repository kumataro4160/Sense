module;

#include <vector>

export module sense.riff_chunk_deserializer;

export import sense.riff_chunk;
export import sense.binary;

namespace sense
{
	uint16_t read2bytes(Binary &data)
	{
		uint16_t ret;
		data.write(&ret, sizeof(uint16_t));
		return ret;
	}

	uint32_t read4bytes(Binary &data)
	{
		uint32_t ret;
		data.write(&ret, sizeof(uint32_t));
		return ret;
	}

	void ignoreChunk(Binary &binary)
	{
		const uint32_t chunkSize = read4bytes(binary);
		binary.seek(chunkSize);
	}

	FmtChunk deserializeFormatChunk(Binary &binary)
	{
		FmtChunk ret;
		ret.chunk_ID = read4bytes(binary);
		if(ret.chunk_ID != ' tmf')
		{
			throw "deserializeFormatChunk : 対象ファイルのfmt チャンクを検出できませんでした。";
		}
		ret.chunk_size = read4bytes(binary);
		if(ret.chunk_size < 16)
		{
			throw "deserializeFormatChunk : 対象ファイルのfmt チャンクサイズが小さすぎます。";
		}
		const uint32_t diff = ret.chunk_size - 16;
		ret.wave_format_type = read2bytes(binary);
		ret.channel = read2bytes(binary);
		ret.samples_per_sec = read4bytes(binary);
		ret.bytes_per_sec = read4bytes(binary);
		ret.block_size = read2bytes(binary);
		ret.bits_per_sample = read2bytes(binary);
		binary.seek(diff);
		return ret;
	}

	DataChunk deserializeDataChunk(Binary &binary)
	{
		if(binary.isEOF())
		{
			throw "deserializeDataChunk : 対象ファイルのdataチャンクを検出できませんでした。";
		}
		DataChunk ret;
		ret.chunk_ID = read4bytes(binary);
		if(ret.chunk_ID != 'atad')
		{
			ignoreChunk(binary);
			return deserializeDataChunk(binary);
		}
		ret.chunk_size = read4bytes(binary);
		ret.signalData.resize(ret.chunk_size);
		binary.write(&ret.signalData[0], ret.signalData.size());
		return ret;
	}

	export RIFFChunk deserializeRIFFChunk(Binary &binary)
	{
		RIFFChunk ret;
		ret.chunk_ID = read4bytes(binary);
		if(ret.chunk_ID != 'FFIR')
		{
			throw "deserializeRIFFChunk : 対象ファイルのRIFFチャンクを検出できませんでした。";
		}
		ret.chunk_size = read4bytes(binary);
		ret.format_type = read4bytes(binary);
		ret.fmt_chunk = deserializeFormatChunk(binary);
		ret.data_chunk = deserializeDataChunk(binary);
		return ret;
	}
}