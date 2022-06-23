#pragma once

#include <mbedtls/sha1.h>

class SHA1
{
public:
	SHA1();
	~SHA1();

	void Update(const std::vector<uint8_t>& data);
	void Update(std::string_view data);
	std::string HexDigest();
private:
	mbedtls_sha1_context m_Context;
};
