/*
* File: AppManager.cpp
* Class: AppManager
* Created: 20211024
* Author: SonTV
*/
#include "AppManager.h"
#include "log/LogDefine.h"
#include "tcp/TcpServer.h"
#include "tcp/TcpClient.h"
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
    if (m_iRunningMode == RUNNING_MODE_SERVER)
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
    else if (m_iRunningMode == RUNNING_MODE_CLIENT)
    {
        auto pCompIter = m_hmComponent.find(TCP_CLIENT_COMP);
        if (pCompIter != m_hmComponent.end())
        {
            auto pClient = (tcp::TcpClient*) pCompIter->second.get();
            if (pClient)
            {
                std::string strAuthorizedMsg = "";
                char* szMessage = NULL;
                pClient->AttachAuthortoMessage(m_obProfile.GetAlias(), strMessage, strAuthorizedMsg);
                szMessage = new char[strAuthorizedMsg.size()];
                memcpy(szMessage, strAuthorizedMsg.c_str(), strAuthorizedMsg.size());
                MessagePtr pMessage = std::make_shared<tcp::Message>(strAuthorizedMsg.size(), szMessage, 0);
                pClient->SendMessage(pMessage);
            }
        }
    }
}

void AppManager::RegisterComponents()
{
    std::shared_ptr<core::op::Engine> pEngine = std::make_shared<core::op::Engine>();
    m_hmComponent[pEngine->GetID()] = pEngine;
    if (m_iRunningMode == RUNNING_MODE_SERVER)
    {
        TSLOG(tslog::LL_DEBUG, "[App] Register server");
        std::shared_ptr<tcp::TcpServer> pTcpServer = std::make_shared<tcp::TcpServer>();
        m_hmComponent[pTcpServer->GetID()] = pTcpServer;
    }
    else if (m_iRunningMode == RUNNING_MODE_CLIENT)
    {
        TSLOG(tslog::LL_DEBUG, "[App] Register client");
        std::shared_ptr<tcp::TcpClient> pTcpClient = std::make_shared<tcp::TcpClient>();
        m_hmComponent[pTcpClient->GetID()] = pTcpClient;
    }
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
    core::op::Engine *pEngine = NULL;
    auto pEngineIter = m_hmComponent.find(ENGINE_COMP);

    if (pEngineIter != m_hmComponent.end())
    {
        pEngine = (core::op::Engine*) pEngineIter->second.get();
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
        jServiceInfo = jServiceConfig["service_info"].array();
        // Loop and register service
        for (auto const &obValue : jServiceInfo)
        {
            TSServicePtr pService = NULL;
            strSerivceName = obValue["name"];
            if (strSerivceName == "authenticate")
            {
                pService = std::make_shared<service::AuthenticateService>();
            }
            else if (strSerivceName == "login")
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

}

void AppManager::SetProfile(const std::string &strAlias/* = ""*/)
{
    if (!strAlias.empty())
    {
        m_obProfile.SetAlias(strAlias);
    }
    else
    {
        std::string strBuffer = "";
        while (true)
        {
            printf("Enter your name: ");
            std::getline(std::cin, strBuffer);
            if (!strBuffer.empty())
            {
                m_obProfile.SetAlias(strBuffer);
                break;
            }
        }
    }
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