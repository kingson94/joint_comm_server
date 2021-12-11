/*
* File: Thread.cpp
* Class: TSThread
* Created: 20211127
* Author: SonTV
*/
#include "Thread.h"
#include "util/Utils.h"
namespace core
{
namespace base
{
TSThread::TSThread(const int &iThreadID) : m_iThreadID(iThreadID), m_pThread(NULL)
{
}

TSThread::~TSThread()
{
}

void TSThread::Start()
{
    if (m_pThread == NULL)
    {
        m_pThread.reset(new std::thread(std::bind(&TSThread::Run, this)));
    }
    else
    {
        TSLOG(tslog::LL_DEBUG, "[Base] Cannot init thread multiple times");
    }
}

void TSThread::Join()
{
    if (m_pThread)
    {
        m_pThread->join();
    }
}
} // namespace base
} // namespace core