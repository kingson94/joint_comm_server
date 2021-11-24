/*
* File: AppManager.cpp
* Class: AppManager
* Created: 20211024
* Author: SonTV
*/

#include "AppManager.h"
#include "util/Define.h"
#include "tcp/TcpServer.h"
#include "tcp/TcpClient.h"
#include "core/operator/Engine.h"

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
            auto pServer = (tcp::TcpServer*) pCompIter->second;
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
            auto pClient = (tcp::TcpClient*) pCompIter->second;
            if (pClient)
            {
                std::string strAuthorizedMsg = "";
                pClient->AttachAuthortoMessage(m_obProfile.GetAlias(), strMessage, strAuthorizedMsg);
                pClient->SendMessage(strAuthorizedMsg);
            }
        }
    }
}

void AppManager::RegisterComponents()
{
    if (m_iRunningMode == RUNNING_MODE_SERVER)
    {
        LOG("[App] Register server");
        core::base::Component *pTcpServer = new tcp::TcpServer();
        m_hmComponent[pTcpServer->GetID()] = pTcpServer;
    }
    else if (m_iRunningMode == RUNNING_MODE_CLIENT)
    {
        LOG("[App] Register client");
        core::base::Component *pTcpClient = new tcp::TcpClient();
        m_hmComponent[pTcpClient->GetID()] = pTcpClient;
    }

    core::base::Component *pEngine = new core::op::Engine();
    m_hmComponent[pEngine->GetID()] = pEngine;
}

core::base::Component* AppManager::GetComponent(const std::string &strComponentID)
{
    auto pIter = m_hmComponent.find(strComponentID);
    if (pIter != m_hmComponent.end())
    {
        return pIter->second;
    }

    return NULL;
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
    for (auto &pItComponent : m_hmComponent)
    {
        SAFE_DEL(pItComponent.second);
    }
}
}