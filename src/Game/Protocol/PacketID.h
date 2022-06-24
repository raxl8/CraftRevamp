#pragma once

enum PacketID
{
	Handshake			= 0x00,
	Disconnect			= 0x00,
	EncryptionRequest	= 0x01,
	EncryptionResponse	= 0x01,
	LoginSuccess		= 0x02,
	SetCompression		= 0x03,
};
