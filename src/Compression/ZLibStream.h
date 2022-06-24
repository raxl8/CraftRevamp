#pragma once

#include <zlib.h>

class ZLibStream
{
public:
	std::vector<uint8_t> Compress(const std::vector<uint8_t>& data);
	std::vector<uint8_t> Uncompress(const std::vector<uint8_t>& data);

private:
	z_stream m_Stream;
};
