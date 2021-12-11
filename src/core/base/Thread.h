/*
* File: Thread.h
* Class: TSThread
* Created: 20211106
* Author: SonTV
*/
#pragma once
#include <string>
#include <thread>
#include <memory>

typedef std::shared_ptr<std::thread> ThreadPtr;

namespace core
{
namespace base
{
class TSThread
{
protected:
    int m_iThreadID;
    ThreadPtr m_pThread;

public:
    explicit TSThread(const int &iThreadID);
    ~TSThread();

    virtual void Run() = 0;
    void Start();
    void Join();
};
} // namespace base
} // namespace core
