#include "pch.h"

#include "Protocol.h"

#include <fmt/printf.h>
#include <cpr/cpr.h>

#include <random>

#include "Game/Player.h"
#include "Game/Protocol/PacketID.h"
#include "Crypto/SHA1.h"

Protocol::Protocol(Player* player)
	: m_Player(player), m_State(ConnectionState::Handshaking), m_Encrypted(false)
{
	m_Keypair = std::make_unique<RSAKeypair>(1024);
	
	// 128 - 11, client sends a 128 bytes PKCS#1 v1.5 padded buffer
	// PKCS#1 v1.5 requires 11 bytes empty
	// so 117 is the max size
	m_VerifyToken.resize(117);

	std::random_device randomDevice;
	std::default_random_engine randomEngine(randomDevice());
	std::uniform_int_distribution<int> uniformDist(std::numeric_limits<uint8_t>::min(), std::numeric_limits<uint8_t>::max());
	std::generate(m_VerifyToken.begin(), m_VerifyToken.end(), [&uniformDist, &randomEngine]()
		{
			return uniformDist(randomEngine);
		}
	);
}

void Protocol::HandlePacket(std::vector<uint8_t> data)
{
	if (m_Encrypted && m_EncryptionStream)
	{
		data = m_EncryptionStream->Decrypt(data);
	}

	PacketStream packet(data);
	while (packet.GetCursor() < data.size())
	{
		auto length = packet.ReadVar<int>();
		if ((data.size() - packet.GetCursor()) < length)
			return;

		auto packetId = packet.ReadVar<int>();
		if (packetId == PacketID::Handshake)
		{
			if (m_State == ConnectionState::Handshaking)
			{
				if (!Handshake(packet))
					return;
			}
			else if (m_State == ConnectionState::LoggingIn)
			{
				if (!LoginStart(packet))
					return;
			}
		}
		else if (packetId == PacketID::EncryptionResponse)
		{
			if (!EncryptionResponse(packet))
				return;
		}
	}
}

void Protocol::SendPacket(PacketStream&& packet)
{
	auto buffer = packet.AsBuffer();
	if (m_Encrypted && m_EncryptionStream)
	{
		buffer = m_EncryptionStream->Encrypt(buffer);
	}

	m_Player->SendData(std::move(buffer));
}

void Protocol::Kick(std::string reason /*= ""*/)
{
	if (!reason.empty())
	{
		PacketStream packet;
		packet.WriteVar<int>(PacketID::Disconnect);
		packet.WriteString(fmt::sprintf("{\"text\":\"%s\"}", reason));
		SendPacket(std::move(packet));
	}

	m_Player->Disconnect();
}

bool Protocol::Handshake(PacketStream& packet)
{
	auto protocolVersion = packet.ReadVar<int>();
	if (protocolVersion != 0x2F) // 1.8
	{
		Kick("Unuspported protocol version");
		return false;
	}

	// Address string used to connect
	packet.ReadString();
	// Port used to connect
	packet.Read<uint16_t>();

	auto nextState = packet.ReadVar<int>();
	if (nextState == 2) // Login
	{
		m_State = ConnectionState::LoggingIn;
	}

	return true;
}

bool Protocol::LoginStart(PacketStream& packet)
{
	auto username = packet.ReadString();
	fmt::printf("username -> %s\n", username);
	m_Player->SetUsername(std::move(username));

	PacketStream encryptionPacket;
	encryptionPacket.WriteVar<int>(EncryptionRequest);
	// Server ID
	encryptionPacket.WriteString("");

	auto publicKey = m_Keypair->GetPublicKey();
	encryptionPacket.WriteArray(publicKey);
	encryptionPacket.WriteArray(m_VerifyToken);

	SendPacket(std::move(encryptionPacket));
	return true;
}

bool Protocol::EncryptionResponse(PacketStream& packet)
{
	auto sharedSecretEncrypted = packet.ReadArray();

	auto verifyTokenEncrypted = packet.ReadArray();
	auto verifyToken = m_Keypair->Decrypt(verifyTokenEncrypted);
	if (verifyToken != m_VerifyToken)
	{
		// Cannot send a Disconnect packet as client expect encryption
		// from this point forward, therefor reason should be empty
		Kick();
		return false;
	}

	auto sharedSecret = m_Keypair->Decrypt(sharedSecretEncrypted);
	m_EncryptionStream = std::make_unique<AESCFB8Stream>();
	m_EncryptionStream->SetKeys(sharedSecret, sharedSecret);
	m_Encrypted = true;

	auto sha1 = SHA1();
	// Server ID
	// Shared Secret
	sha1.Update(sharedSecret);
	// Public Key
	sha1.Update(m_Keypair->GetPublicKey());

	auto res = cpr::Get(
		cpr::Url{ "https://sessionserver.mojang.com/session/minecraft/hasJoined" },
		cpr::Parameters{ {"username", m_Player->GetUsername()}, {"serverId", sha1.HexDigest()} },
		cpr::VerifySsl{false});

	printf("%d\n", res.status_code);
	printf("%s\n", res.text.c_str());

	return true;
}
