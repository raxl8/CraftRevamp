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
			auto stream = std::make_shared<TCPStream>(this);

			std::shared_ptr<uvw::TCPHandle> streamHandle = m_Loop->resource<uvw::TCPHandle>();
			serverHandle.accept(*streamHandle);

			stream->Start(std::move(streamHandle));

			m_Streams.insert(stream);

			OnStreamCreated(stream);
		}
	);

	m_Handle->bind(address, port);
	m_Handle->listen();
	m_Loop->run();
}

void TCPServer::RemoveStream(const std::shared_ptr<TCPStream>& stream)
{
	m_Streams.erase(stream);
}
