#pragma once

#include <uvw.hpp>

#include "Core/Callback.h"

class TCPServer;
class TCPStream : public std::enable_shared_from_this<TCPStream>
{
public:
	TCPStream(TCPServer* server);
	~TCPStream();

	void Start(std::shared_ptr<uvw::TCPHandle>&& handle);
	void Stop();

	void Write(std::unique_ptr<char>&& data, size_t size);

	Callback<std::vector<uint8_t>, size_t> OnDataReceived;
	Callback<> OnDisconnect;

private:
	TCPServer* m_Server;
	std::shared_ptr<uvw::TCPHandle> m_Handle;
};
