#include "pch.h"

#include "ZLibStream.h"

#include <cassert>

#define CHUNK 16384

std::vector<uint8_t> ZLibStream::Compress(const std::vector<uint8_t>& data)
{
	m_Stream.zalloc = Z_NULL;
	m_Stream.zfree = Z_NULL;
	m_Stream.opaque = Z_NULL;
	int ret = deflateInit(&m_Stream, Z_DEFAULT_COMPRESSION);
	if (ret != Z_OK)
		return {};

	std::vector<uint8_t> compressed;
	std::vector<uint8_t> chunk(CHUNK);
	int flush = Z_NO_FLUSH;
	uint32_t cursor = 0;
	uint32_t readSize = CHUNK;
	do 
	{
		if ((cursor + CHUNK) >= data.size())
		{
			readSize = data.size() - cursor;
			flush = Z_FINISH;
		}

		m_Stream.avail_in = readSize;
		m_Stream.next_in = (Bytef*)&data[cursor];
		cursor += readSize;

		do
		{
			m_Stream.avail_out = CHUNK;
			m_Stream.next_out = &chunk[0];
			int ret = deflate(&m_Stream, flush);
			assert(ret != Z_STREAM_ERROR);

			auto oldSize = compressed.size();
			auto compressedChunkSize = (CHUNK - m_Stream.avail_out);
			compressed.resize(oldSize + compressedChunkSize);
			std::memcpy(&compressed[oldSize], &chunk[0], compressedChunkSize);
		} while (m_Stream.avail_out == 0);
	} while (flush != Z_FINISH);

	deflateEnd(&m_Stream);

	return compressed;
}

std::vector<uint8_t> ZLibStream::Uncompress(const std::vector<uint8_t>& data)
{
	ZeroMemory(&m_Stream, sizeof(m_Stream));
	int ret = inflateInit(&m_Stream);
	if (ret != Z_OK)
		return {};

	std::vector<uint8_t> uncompressed;
	std::vector<uint8_t> chunk(CHUNK);
	int flush = Z_NO_FLUSH;
	uint32_t cursor = 0;
	uint32_t readSize = CHUNK;
	do
	{
		if ((cursor + CHUNK) >= data.size())
		{
			readSize = data.size() - cursor;
			flush = Z_FINISH;
		}

		m_Stream.avail_in = readSize;
		m_Stream.next_in = (Bytef*)&data[cursor];
		cursor += readSize;

		do
		{
			m_Stream.avail_out = CHUNK;
			m_Stream.next_out = &chunk[0];
			int ret = inflate(&m_Stream, flush);
			assert(ret != Z_STREAM_ERROR);

			auto oldSize = uncompressed.size();
			auto uncompressedChunkSize = (CHUNK - m_Stream.avail_out);
			uncompressed.resize(oldSize + uncompressedChunkSize);
			std::memcpy(&uncompressed[oldSize], &chunk[0], uncompressedChunkSize);
		} while (m_Stream.avail_out == 0);
	} while (flush != Z_FINISH);

	inflateEnd(&m_Stream);

	return uncompressed;
}

