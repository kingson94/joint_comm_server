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

ComponentPtr AppManager::GetComponent(const std::string &strComponentID)
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
}
}