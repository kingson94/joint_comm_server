#pragma once

/*
* File: Engine.h
* Class: Engine
* Created: 20211127
* Author: SonTV
*/
#include "core/base/Component.h"
#include "boost/thread/mutex.hpp"
#include "boost/thread.hpp"
#include <condition_variable>
#include <vector>
#include <unordered_map>
#include "service/connection/TcpReadService.h"
// #include "service/TcpService.h"
#include <memory>
#include "Worker.h"

namespace core
{
namespace base
{
class Task;
}
}

namespace core
{
namespace op
{
class Engine : public core::base::Component
{
private:
    // Read/Write mutexes
    boost::mutex m_mtxQueueWrite;
    boost::mutex m_mtxQueueRead;

    // Full/Empty conditions
    boost::condition_variable m_cvQueueFull;
    boost::condition_variable m_cvQueueEmpty;

    core::base::Task **m_pQueueBegin;
    core::base::Task **m_pQueueEnd;
    core::base::Task **m_pFirstTask;
    core::base::Task **m_pLastTask;
    core::base::Task **m_pNextTask;

    int m_iWorkerCount;
    int m_iQueueSize;
    std::vector<WorkerPtr> m_vWorker;
    std::unordered_map<int, ServicePtr> m_hmService;

public:
    Engine();
    ~Engine();

    void Init() override;
    void Run() override;
    void Join() override;
    void ConsumeTask();
    void PushTask(core::base::Task *pTask);
    core::base::Task* GetTask();

    void RegisterService(ServicePtr pService);
};
} // namespace op
} // namespace core