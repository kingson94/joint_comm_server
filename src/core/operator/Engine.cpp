/*
* File: Engine.cpp
* Class: Engine
* Created: 20211127
* Author: SonTV
*/
#include "Engine.h"
#include "core/base/Task.h"
#include "core/operator/Worker.h"
#include "util/Utils.h"
#include "AppDefine.h"
#include "service/ServiceDefine.h"

namespace core
{
namespace op
{
Engine::Engine() : TSComponent(ENGINE_COMP)
, m_pFirstTask(NULL), m_pLastTask(NULL), m_pNextTask(NULL)
, m_pQueueBegin(NULL), m_pQueueEnd(NULL), m_iWorkerCount(DEFAULT_WORKER_COUNT)
, m_iQueueSize(DEFAULT_QUEUE_SIZE)
{
}

Engine::~Engine()
{
    SAFE_DEL(m_pQueueBegin);
}

void Engine::Join()
{
    // Do nothing
}

void Engine::Init()
{
    nlohmann::json jConfig;
    if (util::Utils::LoadJsonFromFile(ENGINE_CONFIG_FILE, jConfig) == 0)
    {
        m_iWorkerCount = jConfig["worker_count"].get<int>();
        m_iQueueSize = jConfig["queue_size"].get<int>();
    }

    m_pQueueBegin = (base::TSTask**) new base::TSTask*[m_iQueueSize];
    m_pQueueEnd = m_pQueueBegin + m_iQueueSize;

    m_pFirstTask = m_pQueueBegin;
    m_pLastTask = m_pFirstTask;
    m_pNextTask = m_pLastTask + 1;

    for (int i = 0; i < m_iWorkerCount; i++)
    {
        WorkerPtr pWorker = std::make_shared<Worker>(i);
        m_vWorker.push_back(pWorker);
    }
}

void Engine::RegisterService(TSServicePtr pService)
{
    if (pService)
    {
        m_hmService[pService->GetID()] = pService;
    }
}

void Engine::Run()
{
    for (size_t i = 0; i < (size_t)m_iWorkerCount; i++)
    {
        WorkerPtr &pWorker = m_vWorker.at(i);
        pWorker->Start();
    }
}

void Engine::ConsumeTask()
{
    // Get task
    auto pTask = GetTask();

    // Process task
    if (pTask != NULL)
    {
        auto pIter = m_hmService.find(pTask->GetType());
        if (pIter != m_hmService.end())
        {
            auto pContext = pTask->GetContext();
            if (pContext)
            {
                bool bResult = pIter->second->ProcessRequest(pContext);
                if (!bResult)
                {
                    TSLOG(tslog::LL_DEBUG, "[OP] Processing task failed")
                }
            }
        }
        else
        {
            TSLOG2(tslog::LL_DEBUG, "[OP] Receive strange request %d", pTask->GetType());
        }
        
        SAFE_DEL(pTask);
    }
}

void Engine::PushTask(core::base::TSTask *pTask)
{
    bool bQueueIsEmpty = false;

    // Enqueue
    std::unique_lock<std::mutex> lckWrite(m_mtxQueueWrite);
    while (m_pNextTask == m_pFirstTask)
    {
        // Wait here if queue is full
        m_cvQueueFull.wait(lckWrite);
    }

    // Check queue is empty to notify empty condition
    if (m_pFirstTask == m_pLastTask)
    {
        bQueueIsEmpty = true;
    }

    // Write task to the last
    *m_pLastTask = pTask;
    // Move pointer to the next write
    m_pLastTask = m_pNextTask;
    // Prepare for the next write location
    m_pNextTask++;
    // Round the queue
    if (m_pNextTask == m_pQueueEnd)
    {
        m_pNextTask = m_pQueueBegin;
    }

    // Notify empty condition if any waiting
    if (bQueueIsEmpty)
    {
        m_cvQueueEmpty.notify_one();
    }
}

core::base::TSTask* Engine::GetTask()
{
    // Dequeue
    bool bQueueIsFull = false;
    core::base::TSTask* pRetTask = NULL;

    std::unique_lock<std::mutex> lckRead(m_mtxQueueRead);
    while (m_pFirstTask == m_pLastTask)
    {
        // Wait here if queue is empty
        m_cvQueueEmpty.wait(lckRead);
    }

    // Check queue is full to notify full condition
    if (m_pFirstTask == m_pNextTask)
    {
        bQueueIsFull = true;
    }

    // Get the first task
    pRetTask = *m_pFirstTask;
    // Move to the next read
    m_pFirstTask++;

    // Round the queue
    if (m_pFirstTask == m_pQueueEnd)
    {
        m_pFirstTask = m_pQueueBegin;
    }

    // Notify full condition if any wait
    if (bQueueIsFull)
    {
        m_cvQueueFull.notify_one();
    }

    return pRetTask;
}
} // namespace op
} // namespace core