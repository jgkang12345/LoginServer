#include "pch.h"
#include "LoginJobQueue.h"

void LoginJobQueue::Push(const LoginJobObject& data)
{
    _jobQueue.Push(data);
}

void LoginJobQueue::Pop()
{
    _jobQueue.Pop();
}

const LoginJobObject& LoginJobQueue::Front()
{
    return _jobQueue.Front();
}

bool LoginJobQueue::SwapQueue()
{
    return _jobQueue.SwapQueue();
}

bool LoginJobQueue::PopQueueEmpty()
{
    return _jobQueue.PopQueueEmpty();
}
