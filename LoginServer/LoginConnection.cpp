#include "pch.h"
#include "LoginConnection.h"
#include "ConnectionContext.h"
#include "PacketHandler.h"

LoginConnection::LoginConnection(const SOCKET& socket, const SOCKADDR_IN& sockAddrIn)
	:Connection(socket,sockAddrIn)
{

}

LoginConnection::LoginConnection() : Connection()
{
}

LoginConnection::~LoginConnection()
{
}

void LoginConnection::OnRecv(Connection* connection, byte* dataPtr, int32 dataLen)
{
	PacketHandler::HandlePacket(static_cast<LoginConnection*>(connection), dataPtr, dataLen);
}

void LoginConnection::OnDisconnect()
{
	ConnectionContext::GetInstance()->RemoveConnection(_connectionId);
}

void LoginConnection::OnConnect()
{
}
