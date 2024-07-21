/*
* File: Engine.cpp
* Class: TSEngine
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
TSEngine::TSEngine() : TSComponent(ENGINE_COMP)
, m_pToReadTask(NULL), m_pToWriteTask(NULL), m_pNextWriteTask(NULL)
, m_pQueueBegin(NULL), m_pQueueEnd(NULL), m_iWorkerCount(DEFAULT_WORKER_COUNT)
, m_iQueueMaxSize(DEFAULT_TASK_QUEUE_MAX_SIZE)
{
}

TSEngine::~TSEngine()
{
    SAFE_DEL(m_pQueueBegin);
}

void TSEngine::Join()
{
    // Do nothing
}

void TSEngine::Init()
{
    nlohmann::json jConfig;
    if (util::Utils::LoadJsonFromFile(ENGINE_CONFIG_FILE, jConfig) == 0)
    {
        m_iWorkerCount = jConfig["worker_count"].get<int>();
        m_iQueueMaxSize = jConfig["queue_size"].get<int>();
    }

    m_pQueueBegin = (base::TSTask**) new base::TSTask*[m_iQueueMaxSize];
    m_pQueueEnd = m_pQueueBegin + m_iQueueMaxSize;

    m_pToReadTask = m_pQueueBegin;
    m_pToWriteTask = m_pToReadTask;
    m_pNextWriteTask = m_pToWriteTask + 1;

    for (int i = 0; i < m_iWorkerCount; i++)
    {
        WorkerPtr pWorker = std::make_shared<Worker>(i);
        m_vWorker.push_back(pWorker);
    }
}

void TSEngine::RegisterService(TSServicePtr pService)
{
    if (pService)
    {
        m_hmService[pService->GetID()] = pService;
    }
}

void TSEngine::Run()
{
    for (size_t i = 0; i < (size_t)m_iWorkerCount; i++)
    {
        WorkerPtr &pWorker = m_vWorker.at(i);
        pWorker->Start();
    }
}

void TSEngine::ConsumeTask()
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

void TSEngine::PushTask(core::base::TSTask *pTask)
{
    bool bQueueIsEmpty = false;

    // Enqueue
    std::unique_lock<std::mutex> lckWrite(m_mtxQueueWrite);
    while (m_pNextWriteTask == m_pToReadTask)
    {
        // Wait here if queue is full
        m_cvQueueFull.wait(lckWrite);
    }

    // Check queue is empty to notify empty condition
    if (m_pToReadTask == m_pToWriteTask)
    {
        bQueueIsEmpty = true;
    }

    // Write task to the last
    *m_pToWriteTask = pTask;
    // Move pointer to the next write
    m_pToWriteTask = m_pNextWriteTask;
    // Prepare for the next write location
    m_pNextWriteTask++;
    // Round the queue
    if (m_pNextWriteTask == m_pQueueEnd)
    {
        m_pNextWriteTask = m_pQueueBegin;
    }

    // Notify empty condition if any waiting
    if (bQueueIsEmpty)
    {
        m_cvQueueEmpty.notify_one();
    }
}

core::base::TSTask* TSEngine::GetTask()
{
    // Dequeue
    bool bQueueIsFull = false;
    core::base::TSTask* pRetTask = NULL;

    std::unique_lock<std::mutex> lckRead(m_mtxQueueRead);
    if (m_pToReadTask == m_pToWriteTask)
    {
        // Wait here if queue is empty
        m_cvQueueEmpty.wait(lckRead);
    }

    // Check queue is full to notify full condition
    if (m_pNextWriteTask == m_pToReadTask)
    {
        bQueueIsFull = true;
    }

    // Get the first task
    pRetTask = *m_pToReadTask;
    // Move to the next read
    m_pToReadTask++;

    // Round the queue
    if (m_pToReadTask == m_pQueueEnd)
    {
        m_pToReadTask = m_pQueueBegin;
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