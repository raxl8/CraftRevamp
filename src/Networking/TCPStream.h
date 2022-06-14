#pragma once

#include <uvw.hpp>

#include "Core/Callback.h"

class TCPServer;
class TCPStream
{
public:
	TCPStream(TCPServer* server);
	~TCPStream() = default;

	void Start(std::shared_ptr<uvw::TCPHandle>&& handle);
	void Stop();

	void Write(std::unique_ptr<char>&& data, size_t size);

	Callback<std::vector<uint8_t>, size_t> OnDataReceived;

private:
	TCPServer* m_Server;
	std::shared_ptr<uvw::TCPHandle> m_Handle;
};
