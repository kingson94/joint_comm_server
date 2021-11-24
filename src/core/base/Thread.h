/*
* File: Thread.h
* Class: BaseThread
* Created: 20211106
* Author: SonTV
*/

#pragma once
#include <string>
#include <thread>
#include <memory>

namespace core
{
namespace base
{
class BaseThread
{
protected:
    int m_iThreadID;
    std::shared_ptr<std::thread> m_pThread;

public:
    explicit BaseThread(const int &iThreadID);
    ~BaseThread();

    virtual void Run() = 0;
    void Start();
    void Join();
};
} // namespace base
} // namespace core