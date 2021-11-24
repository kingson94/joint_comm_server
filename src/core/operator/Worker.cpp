#include "core/operator/Worker.h"
#include "core/operator/Engine.h"
#include "AppManager.h"
#include "util/Utils.h"

namespace core
{
namespace op
{
Worker::Worker(const int &iThreadID) : base::BaseThread(iThreadID)
{

}

Worker::~Worker()
{
}

void Worker::Run()
{
    auto pAppInstance = AppManager::GetInstance();
    if (pAppInstance)
    {
        auto pEngineComp = (core::op::Engine*) pAppInstance->GetComponent(ENGINE_COMP);
        
        if (pEngineComp)
        {
            while (true)
            {
                if (pAppInstance->GetRunningMode() == RUNNING_MODE_SERVER)
                {
                    LOG2("Worker %d start", m_iThreadID);
                }

                pEngineComp->ConsumeTask();

                if (pAppInstance->GetRunningMode() == RUNNING_MODE_SERVER)
                {
                    LOG2("Worker %d end", m_iThreadID);
                }
            }
        }
    }
}
}
} // namespace core