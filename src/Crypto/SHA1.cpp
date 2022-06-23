#include "pch.h"

#include "SHA1.h"

#include <mbedtls/bignum.h>

SHA1::SHA1()
{
	mbedtls_sha1_init(&m_Context);
	mbedtls_sha1_starts(&m_Context);
}

SHA1::~SHA1()
{
	mbedtls_sha1_free(&m_Context);
}

void SHA1::Update(const std::vector<uint8_t>& data)
{
	mbedtls_sha1_update(&m_Context, &data[0], data.size());
}

void SHA1::Update(std::string_view data)
{
	mbedtls_sha1_update(&m_Context, (const uint8_t*)&data[0], data.size());
}

std::string SHA1::HexDigest()
{
	uint8_t output[20];
	int ret = mbedtls_sha1_finish(&m_Context, output);
	if (ret != 0)
		return "";

	mbedtls_sha1_init(&m_Context);

	mbedtls_mpi mpi;
	mbedtls_mpi_init(&mpi);

	ret = mbedtls_mpi_read_binary(&mpi, output, sizeof(output));
	if (ret != 0)
	{
		mbedtls_mpi_free(&mpi);
		return "";
	}

	std::string result;
	if (mbedtls_mpi_get_bit(&mpi, 159) != 0)
	{
		result += '-';

		// 1's compliment
		for (int i = 0; i < sizeof(output); i++)
			output[i] = ~output[i];

		ret = mbedtls_mpi_read_binary(&mpi, output, sizeof(output));
		if (ret != 0)
		{
			mbedtls_mpi_free(&mpi);
			return "";
		}

		// 2's compliment
		mbedtls_mpi tmp;
		mbedtls_mpi_init(&tmp);
		ret = mbedtls_mpi_add_int(&tmp, &mpi, 1);
		mbedtls_mpi_free(&mpi);

		if (ret != 0)
		{
			mbedtls_mpi_free(&tmp);
			return "";
		}

		mpi = tmp;
	}

	char outputHex[sizeof(output) * 2 + 2];
	size_t ouputHexLen;
	ret = mbedtls_mpi_write_string(&mpi, 16, outputHex, sizeof(outputHex), &ouputHexLen);
	mbedtls_mpi_free(&mpi);

	if (ret != 0)
		return "";

	for (int i = 0; i < sizeof(outputHex); i++)
		outputHex[i] = std::tolower(outputHex[i]);

	auto view = std::string_view(outputHex);
	while (view.size() && view[0] == '0')
		view = view.substr(1);

	result.append(view.begin(), view.end());

	return result;
}
