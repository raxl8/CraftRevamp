#pragma once

#include "Game/Protocol/PacketStream.h"
#include "Crypto/RSAKeypair.h"
#include "Crypto/AESCFB8Stream.h"

class Player;
class Protocol
{
	enum class ConnectionState
	{
		Handshaking = 0,
		LoggingIn,
		Play,
	};

public:
	Protocol(Player* player);

	void HandlePacket(std::vector<uint8_t> data);
	void SendPacket(PacketStream&& packet);
	void Kick(std::string reason = "");
	bool Handshake(PacketStream& packet);
	bool LoginStart(PacketStream& packet);
	bool EncryptionResponse(PacketStream& packet);

private:
	Player* m_Player;
	ConnectionState m_State;
	std::unique_ptr<RSAKeypair> m_Keypair;
	std::vector<uint8_t> m_VerifyToken;
	bool m_Encrypted;
	std::unique_ptr<AESCFB8Stream> m_EncryptionStream;
};
