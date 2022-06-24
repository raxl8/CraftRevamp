#pragma once

#include "Networking/TCPStream.h"
#include "Game/Protocol/Protocol.h"
#include "Game/Protocol/PacketStream.h"
#include "Game/UUID.h"

class GameServer;
class Player
{
public:
	Player(GameServer* gameServer, std::shared_ptr<TCPStream> tcpStream);
	void Disconnect();
	void SendData(std::vector<uint8_t>&& data);

	inline void SetUsername(std::string&& username) { m_Username = std::move(username); }
	inline const std::string& GetUsername() { return m_Username; }

	inline void SetUUID(UUID&& uuid) { m_UUID = std::move(uuid); }
	inline const UUID& GetUUID() { return m_UUID; }

private:
	GameServer* m_gameServer;
	std::weak_ptr<TCPStream> m_TCPStream;
	std::unique_ptr<Protocol> m_Protocol;
	std::string m_Username;
	UUID m_UUID;
};
