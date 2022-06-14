#include "pch.h"

#include "Networking/TCPServer.h"

int main()
{
	auto server = new TCPServer;
	server->Listen("127.0.0.1", 25565);
	delete server;
	return 0;
}
