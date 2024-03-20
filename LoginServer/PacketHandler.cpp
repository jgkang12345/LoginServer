#include "pch.h"
#include "PacketHandler.h"
#include "LoginJobQueue.h"
#include "LoginConnection.h"
void PacketHandler::HandlePacket(LoginConnection* connection, BYTE* packet, int32 packetSize)
{
	PacketHeader* header = reinterpret_cast<PacketHeader*>(packet);
	BYTE* dataPtr = packet + sizeof(PacketHeader);
	int32 dataSize = packetSize - sizeof(PacketHeader);

	switch (header->_type)
	{
	case PacketProtocol::C2S_LOGIN: // 플레이어 동기화
		HandlePacket_C2S_LOGIN(connection, dataPtr, dataSize);
		break;
	}
}

void PacketHandler::HandlePacket_C2S_LOGIN(LoginConnection* connection, BYTE* packet, int32 packetSize)
{
	int32 userIdLen;
	int32 userPwLen;

	BinaryReader br(packet);
	LoginJobObject loginJobObject;
	loginJobObject.connection = connection;
	br.Read(loginJobObject.userIdLen);
	br.ReadWString(loginJobObject.userId, loginJobObject.userIdLen);
	br.Read(loginJobObject.userPwLen);
	br.ReadWString(loginJobObject.userPw, loginJobObject.userPwLen);

	LoginJobQueue::GetInstance()->Push(loginJobObject);
}
