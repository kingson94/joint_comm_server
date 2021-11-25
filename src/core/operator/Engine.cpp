#include "Engine.h"
#include "core/base/Task.h"
#include "core/operator/Worker.h"
#include "util/Utils.h"

#include "service/TcpReadService.h"
#include "AppDefine.h"

// #include "service/TcpService.h"


namespace core
{
namespace op
{
Engine::Engine() : Component(ENGINE_COMP)
, m_pFirstTask(NULL), m_pLastTask(NULL), m_pNextTask(NULL)
, m_pQueueBegin(NULL), m_pQueueEnd(NULL), m_iWorkerCount(DEFAULT_WORKER_COUNT)
, m_iQueueSize(DEFAULT_QUEUE_SIZE)
{
    core::base::Service *pReadService = new service::TcpReadService();
    // core::base::Service *pTcpService = new service::TcpService();

    RegisterService(pReadService);
    // RegisterService(pTcpService);
}

Engine::~Engine()
{
    SAFE_DEL(m_pQueueBegin);

    for (auto &ptService : m_hmService)
    {
        SAFE_DEL(ptService.second);
    }

    for (auto &t:m_vWorker)
    {
        t->Join();
    }
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

    m_pQueueBegin = (base::Task**) new base::Task*[m_iQueueSize];
    m_pQueueEnd = m_pQueueBegin + m_iQueueSize;

    m_pFirstTask = m_pQueueBegin;
    m_pLastTask = m_pFirstTask;
    m_pNextTask = m_pLastTask + 1;

    for (int i = 0; i < m_iWorkerCount; i++)
    {
        std::shared_ptr<Worker> pWorker = std::make_shared<Worker>(i);
        m_vWorker.push_back(pWorker);
    }
}

void Engine::RegisterService(core::base::Service *pService)
{
    if (pService)
    {
        m_hmService[pService->GetType()] = pService;
    }
}

void Engine::Run()
{
    for (size_t i = 0; i < (size_t)m_iWorkerCount; i++)
    {
        std::shared_ptr<Worker> &pWorker = m_vWorker.at(i);
        pWorker->Start();
    }
}

void Engine::ConsumeTask()
{
    // SLOG(slog::LL_DEBUG, "[Engine] Consume task");
    // Get task
    auto pTask = GetTask();

    // Process task
    if (pTask != NULL)
    {
        // SLOG2(slog::LL_DEBUG, "[Engine] Got a task has type: %d", pTask->GetType());
        auto pIter = m_hmService.find(pTask->GetType());
        if (pIter != m_hmService.end())
        {
            auto pContext = pTask->GetContext();
            if (pContext)
            {
                bool bResult = pIter->second->ProcessRequest(pContext);
                if (!bResult)
                {
                    SLOG(slog::LL_DEBUG, "[Engine] Processing task failed")
                }
            }
        }
        else
        {
            SLOG2(slog::LL_DEBUG, "[Engine] Receive strange request %d", pTask->GetType());
        }
        
        SAFE_DEL(pTask);
    }
}

void Engine::PushTask(core::base::Task *pTask)
{
    bool bQueueIsEmpty = false;
    // Enqueue
    std::unique_lock<std::mutex> lckWrite(m_mtxQueueWrite);
    if (m_pNextTask == m_pFirstTask)
    {
        // SLOG(slog::LL_DEBUG, "[Engine] Wait when queue is full");
        // Wait here if queue is full
        m_cvQueueFull.wait(lckWrite);
    }

    // Check queue is empty to notify empty condition
    if (m_pFirstTask == m_pLastTask)
    {
        // SLOG(slog::LL_DEBUG, "[Engine] Queue empty before");
        bQueueIsEmpty = true;
    }

    // SLOG2(slog::LL_DEBUG, "[Engine] Pushed task has type: %d", pTask->GetType());
    // Write task to the last
    *m_pLastTask = pTask;
    // Move pointer to the next write
    m_pLastTask = m_pNextTask;
    // Prepare for the next write location
    m_pNextTask++;
    // Round the queue
    if (m_pNextTask == m_pQueueEnd)
    {
        // SLOG(slog::LL_DEBUG, "[Engine] Push task round the queue");
        m_pNextTask = m_pQueueBegin;
    }

    // Notify empty condition if any waiting
    if (bQueueIsEmpty)
    {
        // SLOG(slog::LL_DEBUG, "[Engine] Notify to empty waiter");
        m_cvQueueEmpty.notify_one();
    }
}

core::base::Task* Engine::GetTask()
{
    // Dequeue
    bool bQueueIsFull = false;
    std::unique_lock<std::mutex> lckRead(m_mtxQueueRead);
    core::base::Task* pRetTask = NULL;
    if (m_pFirstTask == m_pLastTask)
    {
        // Wait here if queue is empty
        // SLOG(slog::LL_DEBUG, "[Engine] Wait queue empty");
        m_cvQueueEmpty.wait(lckRead);
    }

    // Check queue is full to notify full condition
    if (m_pFirstTask == m_pNextTask)
    {
        // SLOG(slog::LL_DEBUG, "[Engine] Queue is full before");
        bQueueIsFull = true;
    }

    // Get the first task
    pRetTask = *m_pFirstTask;
    // Move to the next read
    m_pFirstTask++;

    // Round the queue
    if (m_pFirstTask == m_pQueueEnd)
    {
        // SLOG(slog::LL_DEBUG, "[Engine] Get task round the queue");
        m_pFirstTask = m_pQueueBegin;
    }

    // Notify full condition if any wait
    if (bQueueIsFull)
    {
        m_cvQueueFull.notify_one();
    }

    // SLOG2(slog::LL_DEBUG, "[Engine] Got a task has type: %d", pRetTask->GetType());
    return pRetTask;
}
} // namespace op
} // namespace core