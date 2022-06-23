#include "pch.h"

#include "Game/GameServer.h"

int main()
{
	auto server = new GameServer;
	server->Start();
	delete server;
	return 0;
}
