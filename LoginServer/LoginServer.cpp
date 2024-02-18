#include "pch.h"
#include "JGNet98App.h"
#include "LoginConnection.h"
#include "ThreadManager.h"
#include "IOCPCore.h"
#include "LoginJobQueue.h"
#include "ConnectionContext.h"
unsigned int _stdcall DispatchProc(void* Args)
{
	JGNet98App* app = reinterpret_cast<JGNet98App*>(Args);
	while (true)
		app->GetIOCPCore()->Dispatch();

	return 0;
}

unsigned int _stdcall AcceptProc(void* Args)
{
	JGNet98App* app = reinterpret_cast<JGNet98App*>(Args);
	app->Run(L"LoginServer!");

	return 0;
}

void LoginProc() 
{
	LoginJobQueue* loginJobQueue = LoginJobQueue::GetInstance();
	bool work = loginJobQueue->SwapQueue();
	DBConnection* con = AccountDBConnectionPool::GetInstance()->Pop();
	if (work == false)
	{
		AccountDBConnectionPool::GetInstance()->Push(con);
		return;
	}
	try
	{
		while (loginJobQueue->PopQueueEmpty() == false)
		{
			LoginJobObject loginJobObject = loginJobQueue->Front();
			loginJobQueue->Pop();

			LoginConnection* connection = loginJobObject.connection;

			if (connection == nullptr)
				continue;

			UserInfoObject userInfoObject;
			SQLINTEGER  SQ;
	
			{
				WCHAR buffer[256] = {};
				int32 userIdLen = wcslen(loginJobObject.userId);
				::memcpy(buffer, loginJobObject.userId, sizeof(WCHAR) * userIdLen);

				SQLPrepare(con->GetHSTMT(), (SQLWCHAR*)L"select top 1 USER_ID , USER_PW, SQ from requies.d_user where USER_ID = ? ;", SQL_NTS);
				SQLBindParameter(con->GetHSTMT(), 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_VARCHAR, sizeof(buffer), 0, (SQLWCHAR*)buffer, sizeof(buffer), NULL);

				WCHAR userId2;
				SQLLEN len = 0;
				SQLBindCol(con->GetHSTMT(), 1, SQL_WCHAR, (SQLWCHAR*)&userInfoObject.userId, sizeof(userInfoObject.userId), &len);
				SQLBindCol(con->GetHSTMT(), 2, SQL_WCHAR, (SQLWCHAR*)&userInfoObject.userPw, sizeof(userInfoObject.userPw), &len);
				SQLBindCol(con->GetHSTMT(), 3, SQL_INTEGER, &SQ, sizeof(SQ), &len);

				SQLExecute(con->GetHSTMT());
				SQLFetch(con->GetHSTMT());
				SQLCloseCursor(con->GetHSTMT());
			}

			int32 userIdSize = wcslen(userInfoObject.userId);
			int32 compare = wcscmp(loginJobObject.userPw, userInfoObject.userPw);

			BYTE sendBuffer[1000];
			BinaryWriter bw(sendBuffer);
			PacketHeader* pktHeader = bw.WriteReserve<PacketHeader>();
			int32 canLogin = 9999;

			if (compare != 0)
			{
				// 로그인 실패
				canLogin = 10000;
			}

			// 로그인 성공
			bw.Write(canLogin);
			// 유저 SQ
			bw.Write((int32)SQ);
			bw.Write(ServerPort::FIELD_SERVER);

			pktHeader->_type = PacketProtocol::S2C_LOGIN;
			pktHeader->_pktSize = bw.GetWriterSize();

			connection->Send(sendBuffer, bw.GetWriterSize());
			connection->SetConnectionId(SQ);
			ConnectionContext::GetInstance()->AddConnetion(connection->GetConnectionId(), connection);
			/*
				로그인 로그 남기기
			*/
			{
				WCHAR buffer[256] = {};
				int32 userIdLen = wcslen(loginJobObject.userId);
				::memcpy(buffer, loginJobObject.userId, sizeof(WCHAR) * userIdLen);
				SQLPrepare(con->GetHSTMT(), (SQLWCHAR*)L"update requies.d_user set LOGIN_DT = GETDATE() where USER_ID = ?;", SQL_NTS);
				SQLBindParameter(con->GetHSTMT(), 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_VARCHAR, sizeof(buffer), 0, (SQLWCHAR*)buffer, sizeof(buffer), NULL);
				SQLExecute(con->GetHSTMT());
				SQLFetch(con->GetHSTMT());
				SQLCloseCursor(con->GetHSTMT());
			}
		}
	}
	catch (const char* errorMessage) {
		std::cerr << "Error: " << errorMessage << std::endl;
	}
	AccountDBConnectionPool::GetInstance()->Push(con);
}

int main() 
{
	PlayerDBConnectionPool::GetInstance()->Init(L"PLAYER", L"sa", L"root", 5);
	AccountDBConnectionPool::GetInstance()->Init(L"MSSQL", L"sa", L"root", 5);

	const char* ip = "127.0.0.1";
	uint16 port = 30003;
	JGNet98App loginServerApp(ip,port, LoginConnection::MakeGameSession);

	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);

	int32 threadCount = sysInfo.dwNumberOfProcessors * 2;

	for (int i = 0; i < threadCount; i++)
		ThreadManager::GetInstacne()->Launch(DispatchProc, &loginServerApp);

	ThreadManager::GetInstacne()->Launch(AcceptProc, &loginServerApp);

	while (true) 
	{
		LoginProc();
		Sleep(0);
	}

	ThreadManager::GetInstacne()->AllJoin();

	return 0;
}	