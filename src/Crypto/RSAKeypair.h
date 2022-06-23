#pragma once

#include <mbedtls/entropy.h>
#include <mbedtls/ctr_drbg.h>
#include <mbedtls/pk.h>

class RSAKeypair
{
public:
	RSAKeypair(uint32_t bits);
	~RSAKeypair();

	std::vector<uint8_t> GetPublicKey();
	std::vector<uint8_t> Encrypt(const std::vector<uint8_t>& data);
	std::vector<uint8_t> Decrypt(const std::vector<uint8_t>& data);

private:
	uint32_t m_Bits;
	mbedtls_entropy_context m_EntropyContext;
	mbedtls_ctr_drbg_context m_CtrDrbgContext;
	mbedtls_pk_context m_Context;
};
