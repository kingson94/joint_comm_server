/*
* File: AppManager.cpp
* Class: AppManager
* Created: 20211024
* Author: SonTV
*/
#include "AppManager.h"
#include "log/LogDefine.h"
#include "tcp/TcpServer.h"
#include "core/operator/Engine.h"
#include <iostream>
#include "AppDefine.h"
#include "service/ServiceDefine.h"

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

void AppManager::Init()
{
    for (auto &pItComponent : m_hmComponent)
    {
        pItComponent.second->Init();
    }
}

void AppManager::Run()
{ 
    for (auto &pItComponent : m_hmComponent)
    {
        pItComponent.second->Run();
    }

    for (auto &pItComponent : m_hmComponent)
    {
        pItComponent.second->Join();
    }
}

void AppManager::SendMessageToEndpoint(const std::string &strMessage)
{
    auto pCompIter = m_hmComponent.find(TCP_SERVER_COMP);
    if (pCompIter != m_hmComponent.end())
    {
        auto pServer = (tcp::TcpServer*) pCompIter->second.get();
        if (pServer)
        {
            // pServer->SendMessage(strMessage);
            return;
        }
    }
}

void AppManager::RegisterComponents()
{
    std::shared_ptr<core::op::TSEngine> pEngine = std::make_shared<core::op::TSEngine>();
    std::shared_ptr<tcp::TcpServer> pTcpServer = std::make_shared<tcp::TcpServer>();

    m_hmComponent[pEngine->GetID()] = pEngine;
    m_hmComponent[pTcpServer->GetID()] = pTcpServer;
}

TSComponentPtr AppManager::GetComponent(const std::string &strComponentID)
{
    auto pIter = m_hmComponent.find(strComponentID);
    if (pIter != m_hmComponent.end())
    {
        return pIter->second;
    }

    return NULL;
}

int AppManager::RegisterService()
{
    nlohmann::json jServiceConfig;
    nlohmann::json jServiceInfo;
    std::string strSerivceName = "";
    int iServiceID = -1;
    core::op::TSEngine *pEngine = NULL;
    auto pEngineIter = m_hmComponent.find(ENGINE_COMP);

    if (pEngineIter != m_hmComponent.end())
    {
        pEngine = (core::op::TSEngine*) pEngineIter->second.get();
        if (!pEngine)
        {
            TSLOG2(tslog::LL_ERROR, "[App] Engine component not found");
            return 1;
        }
    }

    if (util::Utils::LoadJsonFromFile(SERVICE_CONFIG_FILE, jServiceConfig) != 0)
    {
        TSLOG2(tslog::LL_ERROR, "[App] Cannot load config file: %s", SERVICE_CONFIG_FILE);
        return 2;
    }

    // Register read socket
    {
        auto pSocketReadService = std::make_shared<service::SocketReadService>();
        pSocketReadService->SetID(SOCKET_READ_SERVICE_ID);
        pEngine->RegisterService(pSocketReadService);
    }
    
    try
    {
        jServiceInfo = jServiceConfig["service_info"];
        // Loop and register service
        for (auto const &obValue : jServiceInfo)
        {
            TSServicePtr pService = NULL;
            strSerivceName = obValue["name"];
            if (strSerivceName == "login")
            {
                pService = std::make_shared<service::LoginService>();
            }

            pService->SetID(obValue["id"]);
            pEngine->RegisterService(pService);
        }
    }
    catch (std::exception &ex)
    {
        TSLOG2(tslog::LL_ERROR, "[App] Reading json file get error: %s", ex.what());
        return 3;
    }

    return 0;
}

void AppManager::SetRunningMode(const int &iMode)
{
    m_iRunningMode = iMode;
}

int AppManager::GetRunningMode() const
{
    return m_iRunningMode;
}

AppManager::~AppManager()
{
}
}