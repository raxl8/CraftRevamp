#include "pch.h"

#include "AESCFB8Stream.h"

AESCFB8Stream::AESCFB8Stream(const std::vector<uint8_t>& key, const std::vector<uint8_t>& iv)
	: m_Key(key), m_IV(iv)
{
	mbedtls_aes_init(&m_Context);

	m_Key = key;
	m_IV = iv;

	mbedtls_aes_setkey_dec(&m_Context, m_Key.data(), (unsigned int)m_Key.size() * 8);
	mbedtls_aes_setkey_enc(&m_Context, m_Key.data(), (unsigned int)m_Key.size() * 8);
}

AESCFB8Stream::~AESCFB8Stream()
{
	mbedtls_aes_free(&m_Context);
}

std::vector<uint8_t> AESCFB8Stream::Encrypt(const std::vector<uint8_t>& data)
{
	std::vector<uint8_t> encrypted(data.size());
	int ret = mbedtls_aes_crypt_cfb8(&m_Context, MBEDTLS_AES_ENCRYPT, data.size(), &m_IV[0], &data[0], &encrypted[0]);
	if (ret != 0)
		return {};

	return encrypted;
}

std::vector<uint8_t> AESCFB8Stream::Decrypt(const std::vector<uint8_t>& data)
{
	std::vector<uint8_t> decrypted(data.size());
	int ret = mbedtls_aes_crypt_cfb8(&m_Context, MBEDTLS_AES_DECRYPT, data.size(), &m_IV[0], &data[0], &decrypted[0]);
	if (ret != 0)
		return {};

	return decrypted;
}
