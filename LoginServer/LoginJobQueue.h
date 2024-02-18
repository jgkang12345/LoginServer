#pragma once
#include "ThreadSafeQueue.h"
class LoginJobQueue
{
private:
	ThreadSafeQueue<LoginJobObject> _jobQueue;
	
public:
	static LoginJobQueue* GetInstance() 
	{
		static LoginJobQueue s_instance;
		return &s_instance;
	}

	void Push(const LoginJobObject& data);
	void Pop();
	const LoginJobObject& Front();
	bool SwapQueue();
	bool PopQueueEmpty();
};

