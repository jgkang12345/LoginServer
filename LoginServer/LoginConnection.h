#pragma once
#include "Connection.h"

class LoginConnection : public Connection
{
private:
	WCHAR _username[50] = {};
	int32 _lastServerPort;

public:
	static Connection* MakeGameSession(const SOCKET& socket, const SOCKADDR_IN& sockAddr) { return new LoginConnection(socket, sockAddr); }
	static Connection* MakeGameSession2() { return new LoginConnection(); }

public:
	LoginConnection(const SOCKET& socket, const SOCKADDR_IN& sockAddrIn);
	LoginConnection();
	virtual ~LoginConnection();

public:
	virtual void	OnRecv(Connection* connection, byte* dataPtr, int32 dataLen);
	virtual void	OnDisconnect();
	virtual void    OnConnect();
};

