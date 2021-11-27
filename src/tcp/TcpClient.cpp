/*
* File: TcpClient.cpp
* Class: TcpClient
* Created: 20211127
* Author: SonTV
*/
#include "TcpClient.h"
#include "util/Utils.h"
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>
#include "tcp/Connection.h"
#include "AppDefine.h"

namespace tcp
{
TcpClient::TcpClient() : core::base::Component(TCP_CLIENT_COMP), m_iClientFD(-1), m_strServerHost(""), m_iServerPort(-1)
{
}

TcpClient::~TcpClient()
{
    SAFE_DEL(m_pLauncher);
}

void TcpClient::Run()
{
    if (m_bIsInit)
    {
        if (Connect(m_strServerHost, m_iServerPort) == true)
        {
            SLOG2(slog::LL_DEBUG, "[TcpClient] Connected to host: %s port: %d", m_strServerHost.c_str(), m_iServerPort);
            m_pLauncher = new TcpClientLauncher(102, this);
            m_pLauncher->Start();
        }
        else
        {
            SLOG2(slog::LL_DEBUG, "[TcpClient] Cannot connect to server: %s, port: %d", m_strServerHost.c_str(), m_iServerPort);
        }
    }
}

bool TcpClient::Connect(const std::string &strHost, const int &iPort)
{
    struct sockaddr_in stSocketAddress;

    int iConnectedFD = -1;
    int iResult = 0;

    // Create socket via TCP/IP v4
    iConnectedFD = socket(AF_INET, SOCK_STREAM, 0);
    if (iConnectedFD == -1)
    {
        SLOG(slog::LL_DEBUG, "[TcpClient] Cannot create socket");
        return false;
    }

    memset(&stSocketAddress, 0, sizeof(stSocketAddress));

    stSocketAddress.sin_family = AF_INET;
    if (strHost.empty())
    {
        SLOG(slog::LL_DEBUG, "[TcpClient] Hostname is empty");
        return false;
    }

    stSocketAddress.sin_addr.s_addr = inet_addr(strHost.c_str());
    stSocketAddress.sin_port = htons((uint16_t) iPort);
    
    // Connect socket to address
    iResult = connect(iConnectedFD, (struct sockaddr*) &stSocketAddress, sizeof(stSocketAddress));
    if (iResult != 0)
    {
        SLOG2(slog::LL_DEBUG, "[TcpClient] Cannot connect to address. Error: %d", errno);
        return false;
    }

    auto pConnection = std::make_shared<Connection>(iConnectedFD);
    if (!pConnection)
    {
        SLOG2(slog::LL_DEBUG, "[TcpClient] Cannot create connection on fd: %d", iConnectedFD);
        close(iConnectedFD);
    }

    std::unique_lock<std::mutex> lckConnectionWrite(m_mtxConnectionWrite);
    m_hmConnection[iConnectedFD] = pConnection;
     m_iClientFD = iConnectedFD;
    
    return true;
}

void TcpClient::SendMessage(const std::string &strContent)
{
    if (m_bIsInit)
    {
        if (m_iClientFD > 0)
        {
            for (auto &pIter : m_hmConnection)
            {
                int iWrittenSize = pIter.second->WriteSocket(strContent);
                if (iWrittenSize > 0)
                {
                    SLOG2(slog::LL_DEBUG, "[TcpClient] Write socket %d total size %d", m_iClientFD, iWrittenSize);
                }
                else
                {
                    SLOG2(slog::LL_DEBUG, "[TcpClient] Write socket %d failed", m_iClientFD);
                }
            }
        }
    }
}

void TcpClient::AttachAuthortoMessage(const std::string &strAuthorName, const std::string &strMessageIn, std::string &strMessageOut)
{
    nlohmann::json jMessage;
    jMessage["author"] = strAuthorName;
    jMessage["message"] = strMessageIn;
    strMessageOut = jMessage.dump(4);
}

void TcpClient::Init()
{
    Component::Init();
    // Load local config
    nlohmann::json jConfig;
    if (util::Utils::LoadJsonFromFile(TCP_CLIENT_CONFIG_FILE, jConfig) == 0)
    {
        m_strServerHost = jConfig["host"].get<std::string>();
        m_iServerPort = jConfig["port"].get<int>();
    }
}

void TcpClient::Listen()
{
    // Listen msg from server
    if (m_bIsInit)
    {
        if (m_iClientFD > 0)
        {
            while (true)
            {
                // pConnection->PuskTaskReadSocket();
            }
        }
    }
}

void TcpClientLauncher::Run()
{
    if (m_pClient)
    {
        m_pClient->Listen();
    }
}
} // namespace tcp
