#include "pch.h"

#include "Player.h"

#include <fmt/printf.h>

#include "Game/GameServer.h"

Player::Player(GameServer* gameServer, std::shared_ptr<TCPStream> tcpStream)
	: m_gameServer(gameServer), m_TCPStream(tcpStream)
{
	m_Protocol = std::make_unique<Protocol>(this);

	tcpStream->OnDataReceived.Subscribe([this](std::vector<uint8_t> data)
		{
			m_Protocol->HandlePacket(data);
		}
	);
}

void Player::Disconnect()
{
	auto tcpStreamLocked = m_TCPStream.lock();
	if (tcpStreamLocked)
	{
		tcpStreamLocked->Stop();
	}
}

void Player::SendData(std::vector<uint8_t>&& data)
{
	auto tcpStreamLocked = m_TCPStream.lock();
	if (tcpStreamLocked)
	{
		tcpStreamLocked->Write(std::move(data));
	}
}
