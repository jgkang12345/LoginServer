#pragma once
#include "DBConnection.h"
class AccountDBConnectionPool : public DBConnectionPool
{
public:
	static AccountDBConnectionPool* GetInstance()
	{
		static AccountDBConnectionPool pools;
		return &pools;
	}
};

class PlayerDBConnectionPool : public DBConnectionPool
{
public:
	static PlayerDBConnectionPool* GetInstance()
	{
		static PlayerDBConnectionPool pools;
		return &pools;
	}
};