#pragma once

class LoginConnection;

class PacketHandler
{
public:
	static void HandlePacket(LoginConnection* session, BYTE* packet, int32 packetSize);
private:
	static void HandlePacket_C2S_LOGIN(LoginConnection* session, BYTE* packet, int32 packetSize);
	static void HandlePacket_C2S_CREATECHARACTER(LoginConnection* session, BYTE* packet, int32 packetSize);
	static void HandlePacket_C2S_CHARACTERLIST(LoginConnection* session, BYTE* packet, int32 packetSize);
	static void HandlePacket_C2S_DELETECHARACTER(LoginConnection* session, BYTE* packet, int32 packetSize);
	static void HandlePacket_C2S_GAMEPLAY(LoginConnection* session, BYTE* packet, int32 packetSize);
	static void HandlePacket_C2S_SERVER_MOVE(LoginConnection* session, BYTE* packet, int32 packetSize);
};

