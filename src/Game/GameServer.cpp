#include "pch.h"

#include "GameServer.h"

GameServer::GameServer()
{
	m_TCPServer = std::make_unique<TCPServer>();
}

void GameServer::Start()
{
	m_TCPServer->OnStreamCreated.Subscribe([this](std::shared_ptr<TCPStream> stream)
		{
			auto player = std::make_shared<Player>(this, stream);
			m_PlayersByStream[stream] = player;

			printf("Connected client %p\n", stream.get());
			stream->OnDisconnect.Subscribe([this, stream]()
				{
					m_PlayersByStream.erase(stream);
					printf("Disconnected client %p\n", stream.get());
				}
			);
		}
	);

	// TODO: Config system
	m_TCPServer->Listen("127.0.0.1", 25565);
}

