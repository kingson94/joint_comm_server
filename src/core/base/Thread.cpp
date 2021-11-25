#include "Thread.h"
#include "util/Utils.h"
namespace core
{
namespace base
{
BaseThread::BaseThread(const int &iThreadID) : m_iThreadID(iThreadID), m_pThread(NULL)
{
}

BaseThread::~BaseThread()
{
}

void BaseThread::Start()
{
    if (m_pThread == NULL)
    {
        m_pThread.reset(new std::thread(std::bind(&BaseThread::Run, this)));
    }
    else
    {
        SLOG(slog::LL_DEBUG, "Cannot init thread multiple times");
    }
}

void BaseThread::Join()
{
    if (m_pThread)
    {
        m_pThread->join();
    }
}
} // namespace base
} // namespace core