#pragma once

/*
* File: Engine.h
* Class: TSEngine
* Created: 20211127
* Author: SonTV
*/
#include "core/base/Component.h"
#include <mutex>
#include <thread>
#include <condition_variable>
#include <vector>
#include <unordered_map>
#include "service/ServiceDefine.h"
#include "core/base/Service.h"
#include <memory>
#include "Worker.h"

namespace core
{
namespace base
{
class TSTask;
}
}

namespace core
{
namespace op
{
class TSEngine : public core::base::TSComponent
{
private:
    // Read/Write mutexes
    std::mutex m_mtxQueueWrite;
    std::mutex m_mtxQueueRead;

    // Full/Empty conditions
    std::condition_variable m_cvQueueFull;
    std::condition_variable m_cvQueueEmpty;

    core::base::TSTask **m_pQueueBegin;
    core::base::TSTask **m_pQueueEnd;
    core::base::TSTask **m_pToReadTask;
    core::base::TSTask **m_pToWriteTask;
    core::base::TSTask **m_pNextWriteTask;

    int m_iWorkerCount;
    int m_iQueueMaxSize;
    std::vector<WorkerPtr> m_vWorker;
    std::unordered_map<int, TSServicePtr> m_hmService;

public:
    TSEngine();
    ~TSEngine();

    void Init() override;
    void Run() override;
    void Join() override;
    void ConsumeTask();
    void PushTask(core::base::TSTask *pTask);
    core::base::TSTask* GetTask();

    void RegisterService(TSServicePtr pService);
};
} // namespace op
} // namespace core