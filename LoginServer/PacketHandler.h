#pragma once

class LoginConnection;

class PacketHandler
{
public:
	static void HandlePacket(LoginConnection* session, BYTE* packet, int32 packetSize);
private:
	static void HandlePacket_C2S_LOGIN(LoginConnection* session, BYTE* packet, int32 packetSize);
};

