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
	void RemoveClient(const std::shared_ptr<TCPStream>& client);

public:
	Callback<std::shared_ptr<TCPStream>> OnClientCreated;

private:
	std::shared_ptr<uvw::Loop> m_Loop;
	std::shared_ptr<uvw::TCPHandle> m_Handle;
	std::set<std::shared_ptr<TCPStream>> m_Clients;
};
