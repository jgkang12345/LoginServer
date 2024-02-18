#pragma once
class LoginConnection;
struct LoginJobObject
{
public:
	int32 userIdLen;
	WCHAR userId[100] = { 0 };
	int32 userPwLen;
	WCHAR userPw[100] = { 0 };
	LoginConnection* connection;
};

struct UserInfoObject
{
public:
	WCHAR userId[250] = {};
	WCHAR userPw[250] = {};
};