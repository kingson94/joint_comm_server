/*
* File: AppManager.cpp
* Class: AppManager
* Created: 20211024
* Author: SonTV
*/

#include "AppManager.h"
#include "utils/Define.h"
#include "core/protocol/TcpServer.h"

namespace core
{
AppManager* AppManager::s_pIntance = NULL;

AppManager* AppManager::GetInstance()
{
    return s_pIntance;
}

int AppManager::CreateInstance()
{
    if (!s_pIntance)
    {
        s_pIntance = new AppManager();
        return 0;
    }

    return 1;
}

int AppManager::DestroyInstance()
{
    SAFE_DEL(s_pIntance);
    return 0;
}

void AppManager::Run()
{
    for (auto &pItComponent : m_hmComponent)
    {
        pItComponent.second->Run();
    }
}

void AppManager::RegisterComponents()
{
    core::protocol::TcpServer *pTcpServer = new core::protocol::TcpServer();
    m_hmComponent[pTcpServer->GetID()] = pTcpServer;
}
}