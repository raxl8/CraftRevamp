#pragma once

#include <set>

#include <uvw.hpp>

#include "Core/Callback.h"
#include "Networking/TCPStream.h"

class TCPServer
{
public:
	TCPServer();
	~TCPServer() = default;

	void Listen(const std::string& address, uint32_t port);
	void RemoveClient(TCPStream* client);

public:
	Callback<TCPStream*> OnClientConnect;

private:
	uint32_t m_Port;
	std::shared_ptr<uvw::Loop> m_Loop;
	std::shared_ptr<uvw::TCPHandle> m_Handle;
	std::set<TCPStream*> m_Clients;
};
