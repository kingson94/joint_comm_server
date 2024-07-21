/*
* File: Worker.cpp
* Class: Worker
* Created: 20211127
* Author: SonTV
*/
#include "core/operator/Worker.h"
#include "core/operator/Engine.h"
#include "AppManager.h"
#include "util/Utils.h"
#include "AppDefine.h"

namespace core
{
namespace op
{
Worker::Worker(const int &iThreadID) : base::TSThread(iThreadID)
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
        auto pEngineComp = (core::op::TSEngine*) pAppInstance->GetComponent(ENGINE_COMP).get();
        
        if (pEngineComp)
        {
            while (true)
            {
                if (pAppInstance->GetRunningMode() == RUNNING_MODE_SERVER)
                {
                    TSLOG2(tslog::LL_DEBUG, "Worker %d ready", m_iThreadID);
                }

                pEngineComp->ConsumeTask();

                if (pAppInstance->GetRunningMode() == RUNNING_MODE_SERVER)
                {
                    TSLOG2(tslog::LL_DEBUG, "Worker %d end", m_iThreadID);
                }
            }
        }
    }
}
}
} // namespace core