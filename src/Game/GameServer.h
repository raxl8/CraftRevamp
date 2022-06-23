#pragma once

#include <unordered_map>

#include "Networking/TCPServer.h"
#include "Game/Player.h"

class GameServer
{
public:
	GameServer();
	~GameServer() = default;

	void Start();

private:
	std::unique_ptr<TCPServer> m_TCPServer;
	std::unordered_map<std::shared_ptr<TCPStream>, std::shared_ptr<Player>> m_PlayersByStream;
};
