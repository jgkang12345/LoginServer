#pragma once
#ifdef _DEBUG
#pragma comment(lib, "Debug\\JGNet98.lib")
#else 
#pragma comment(lib, "Release\\JGNet98.lib")
#endif
#include "CorePch.h"
#include "pkt.h"
#include "LoginServerDBConnectionPool.h"