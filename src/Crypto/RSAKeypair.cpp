#include "pch.h"

#include "RSAKeypair.h"

RSAKeypair::RSAKeypair(uint32_t bits)
	: m_Bits(bits)
{
	mbedtls_pk_init(&m_Context);
	mbedtls_pk_setup(&m_Context, mbedtls_pk_info_from_type(MBEDTLS_PK_RSA));
	mbedtls_entropy_init(&m_EntropyContext);
	mbedtls_ctr_drbg_init(&m_CtrDrbgContext);

	mbedtls_ctr_drbg_seed(&m_CtrDrbgContext, mbedtls_entropy_func, &m_EntropyContext, (const unsigned char*)"", 0);
	mbedtls_rsa_gen_key(mbedtls_pk_rsa(m_Context), mbedtls_ctr_drbg_random, &m_CtrDrbgContext, m_Bits, 65537);
}

RSAKeypair::~RSAKeypair()
{
	mbedtls_ctr_drbg_free(&m_CtrDrbgContext);
	mbedtls_entropy_free(&m_EntropyContext);
	mbedtls_pk_free(&m_Context);
}

std::vector<uint8_t> RSAKeypair::GetPublicKey()
{
	std::vector<uint8_t> publicKey(m_Bits);
	int size = mbedtls_pk_write_pubkey_der(&m_Context, publicKey.data(), publicKey.size());

	auto keyStart = &publicKey[publicKey.size() - size];
	return std::vector<uint8_t>(keyStart, keyStart + size);
}

std::vector<uint8_t> RSAKeypair::Decrypt(const std::vector<uint8_t>& data)
{
	size_t len;
	std::vector<uint8_t> decrypted(data.size());
	int ret = mbedtls_rsa_pkcs1_decrypt(mbedtls_pk_rsa(m_Context), mbedtls_ctr_drbg_random, &m_CtrDrbgContext, &len, &data[0], &decrypted[0], decrypted.size());
	if (ret != 0)
		return {};

	decrypted.resize(len);
	return decrypted;
}
