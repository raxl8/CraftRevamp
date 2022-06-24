#pragma once

#include <mbedtls/aes.h>

class AESCFB8Stream
{
public:
	AESCFB8Stream(const std::vector<uint8_t>& key, const std::vector<uint8_t>& iv);
	~AESCFB8Stream();

	std::vector<uint8_t> Encrypt(const std::vector<uint8_t>& data);
	std::vector<uint8_t> Decrypt(const std::vector<uint8_t>& data);

private:
	mbedtls_aes_context m_Context;
	std::vector<uint8_t> m_Key;
	std::vector<uint8_t> m_IV;
};
