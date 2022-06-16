#include "pch.h"

#include "TCPServer.h"

TCPServer::TCPServer()
{
	m_Loop = uvw::Loop::create();
	m_Handle = m_Loop->resource<uvw::TCPHandle>();
}

void TCPServer::Listen(const std::string& address, uint32_t port)
{
	m_Handle->on<uvw::ErrorEvent>([](const uvw::ErrorEvent&, uvw::TCPHandle&)
		{
			// TODO: Error handling
		}
	);

	m_Handle->on<uvw::ListenEvent>([this](const uvw::ListenEvent& e, uvw::TCPHandle& serverHandle)
		{
			auto client = new TCPStream(this);

			std::shared_ptr<uvw::TCPHandle> clientHandle = m_Loop->resource<uvw::TCPHandle>();
			serverHandle.accept(*clientHandle);

			client->Start(std::move(clientHandle));

			m_Clients.insert(client);

			OnClientConnect(client);
		}
	);

	m_Handle->bind(address, port);
	m_Handle->listen();
	m_Loop->run();
}

void TCPServer::RemoveClient(TCPStream* client)
{
	m_Clients.erase(client);

	OnClientDisconnect(client);

	delete client;
}
