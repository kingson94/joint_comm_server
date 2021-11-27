/*
* File: TcpServer.cpp
* Class: TcpServer
* Created: 20211031
* Author: SonTV
*/
#include "TcpServer.h"
#include "log/LogDefine.h"
#include "util/Utils.h"
#include <sys/socket.h>
#include <netdb.h>
#include <cstring>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include "tcp/Connection.h"
#include <sys/epoll.h>
#include "AppDefine.h"

namespace tcp
{
TcpServer::TcpServer() : Component(TCP_SERVER_COMP), m_pLauncher(NULL), m_iServerFD(-1), m_strHost(""), m_iPort(DEFAULT_TCP_PORT)
{
}

TcpServer::~TcpServer()
{
}

void TcpServer::Run()
{
    // Start TCP server
    if (m_bIsInit)
    {
        m_iEpollFD = epoll_create1(0);
        if (m_iEpollFD == -1)
        {
            throw std::runtime_error("Error in epoll_create");
        }

        if (OpenConnection(m_strHost, m_iPort) == 0)
        {
            SLOG2(slog::LL_DEBUG, "[TcpServer] Tcp server started. Host: %s port %d", m_strHost.c_str(), m_iPort);
            m_pLauncher = new TcpServerLauncher(101, this);
            m_pLauncher->Start();
        }
        else
        {
            SLOG(slog::LL_DEBUG, "[TcpServer] Create socket failed");
        }
    }
}

void TcpServer::Init()
{
    Component::Init();

    // Load tcp config here
    nlohmann::json jConfig;
    if (util::Utils::LoadJsonFromFile(TCP_SERVER_CONFIG_FILE, jConfig) == 0)
    {
        m_strHost = jConfig["host"].get<std::string>();
        m_iPort = jConfig["port"].get<int>();
    }
}

void TcpServer::CloseConnection(const int &iFD)
{
    int iResult = -1;
    if (iFD == -1)
    {
        return;
    }

    std::unique_lock<std::mutex> lckConnectionWrite(m_mtxConnectionWrite);
    iResult = close(iFD);
    if (iResult == 0)
    {
        m_hmConnection.erase(iFD);
    }
}

void TcpServer::Listen()
{
    struct epoll_event* pEpollEvents = NULL;

    pEpollEvents = (epoll_event*) calloc(1000, sizeof(epoll_event));
    while (true)
    {
        int nEventCount = 0;
		nEventCount = epoll_wait(m_iEpollFD, pEpollEvents, 1000, -1);
		for (int i = 0; i < nEventCount; i++)
        {
            if ((pEpollEvents[i].events & EPOLLERR) || (pEpollEvents[i].events & EPOLLHUP))
			{
				CloseConnection(pEpollEvents[i].data.fd);
				continue;
			}
			else if (m_setAcceptedFD.count(pEpollEvents[i].data.fd) > 0) // New incoming fd -> accept
            {
                while (true)
                {
                    int iClientFD = -1;
                    int iResult = -1;
                    int uReadCount = 0;
                    struct sockaddr_in stSocketIn;
                    socklen_t iSize = sizeof(stSocketIn);

                    iClientFD = accept(pEpollEvents[i].data.fd, (struct sockaddr*)&stSocketIn, &iSize);
                    if (iClientFD == -1)
                    {
                        break;
                    }

                    SLOG2(slog::LL_DEBUG, "[TcpServer] Accepted new connection. Fd: %d", iClientFD);
                    iResult = MakeNonBlockingFD(iClientFD);
                    if (iResult == -1)
                    {
                        SLOG2(slog::LL_DEBUG, "[TcpServer] Error on making socket non-blocking %d", iClientFD);
                        CloseConnection(iClientFD);
                        break;
                    }

                    auto pConnection = std::make_shared<Connection>(iClientFD);
                    if (!pConnection)
                    {
                        SLOG2(slog::LL_DEBUG, "[TcpServer] Cannot create connection on fd: %d", iClientFD);
                        CloseConnection(iClientFD);
                    }
                    
                    {
                        struct epoll_event stTmpEpollEvent;
                        stTmpEpollEvent.data.fd = iClientFD;
                        stTmpEpollEvent.events = EPOLLIN | EPOLLET;
                        iResult = epoll_ctl(m_iEpollFD, EPOLL_CTL_ADD, iClientFD, &stTmpEpollEvent);
                    }

                    if (iResult == -1)
                    {
                        SLOG(slog::LL_DEBUG, "[TcpServer] Error in epoll add");
                        CloseConnection(iClientFD);
                        break;
                    }

                    std::unique_lock<std::mutex> lckConnectionWrite;
                    m_hmConnection[iClientFD] = pConnection;
                    // pConnection->PuskTaskReadSocket();
                }
            }
            else // Socket already accepted. Continue reading
            {
                SLOG2(slog::LL_DEBUG, "[TcpServer] Reading from established connection. Fd: %d", pEpollEvents[i].data.fd);
                auto pIter = m_hmConnection.find(pEpollEvents[i].data.fd);
                if (pIter != m_hmConnection.end())
                {
                    auto pConnection = pIter->second;
                    if (pConnection)
                    {
                        pConnection->PuskTaskReadSocket();
                    }
                }
            }
        }
    }

    SAFE_DEL(pEpollEvents);
}

int TcpServer::OpenConnection(const std::string &strHost, const int &iPort)
{
    int iListenFD = CreateBoundSocket(strHost, iPort);
    int iResult = -1;
	struct epoll_event stEpollEvent;

    if (iListenFD == -1)
    {
        SLOG(slog::LL_DEBUG, "[TcpServer] Create and bind socket failed");
        return -1;
    }

    iResult = MakeNonBlockingFD(iListenFD);
	if (iResult == -1)
	{
        SLOG2(slog::LL_DEBUG, "[TcpServer] Error on making socket non-blocking %d", iListenFD);
		CloseConnection(iListenFD);
		return -1;
	}

    iResult = listen(iListenFD, SOMAXCONN);
    if (iResult == -1)
	{
        SLOG2(slog::LL_DEBUG, "[TcpServer] Error on socket listening %d", iListenFD);
		CloseConnection(iListenFD);
		return -1;
	}

	stEpollEvent.data.fd = iListenFD;
	stEpollEvent.events = EPOLLIN | EPOLLET;
	iResult = epoll_ctl(m_iEpollFD, EPOLL_CTL_ADD, iListenFD, &stEpollEvent);
	if (iResult == -1)
	{
		SLOG(slog::LL_DEBUG, "[TcpServer] Error in epoll add");
		return -1;
	}

    m_setAcceptedFD.insert(iListenFD);
    m_iServerFD = iListenFD;

    return 0;
}

void TcpServer::Join()
{
    m_pLauncher->Join();
}

int TcpServer::MakeNonBlockingFD(const int& iFD)
{
    int uFlags = fcntl(iFD, F_GETFL, 0);
    int iResult = -1;

    if (uFlags == -1)
    {
        return -1;
    }

    uFlags |= O_NONBLOCK;
    iResult = fcntl(iFD, F_SETFL, uFlags);
    if (iResult == -1)
    {
        return -1;
    }

    return 0;
}

int TcpServer::CreateBoundSocket(const std::string &strHost, const int &iPort)
{
    struct sockaddr_in stSocketAddress;

    int iListenFD = -1;
    int iResult = 0;
    int bOption = 1;

    // Create socket via TCP/IP v4
    iListenFD = socket(AF_INET, SOCK_STREAM, 0);
    if (iListenFD == -1)
    {
        SLOG(slog::LL_DEBUG, "[TcpServer] Cannot create socket");
        return -1;
    }

    iResult = setsockopt(iListenFD, SOL_SOCKET, SO_REUSEADDR, &(bOption), sizeof(bOption));
    if (iResult == -1)
    {
        SLOG(slog::LL_DEBUG, "[TcpServer] Cannot set socket options");
        return -1;
    }
    memset(&stSocketAddress, 0, sizeof(stSocketAddress));

    stSocketAddress.sin_family = AF_INET;
    if (!strHost.empty())
    {
        stSocketAddress.sin_addr.s_addr = inet_addr(strHost.c_str());
    }
    else
    {
        stSocketAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    }
    stSocketAddress.sin_port = htons((uint16_t) iPort);

    // Bind socket with address
    iResult = bind(iListenFD, (struct sockaddr*) &stSocketAddress, sizeof(stSocketAddress));
    if (iResult != 0)
    {
        SLOG2(slog::LL_DEBUG, "[TcpServer] Cannot bind socket with address");
        return -1;
    }

    return iListenFD;
}

void TcpServer::SendMessage(const std::string &strContent)
{
    if (m_bIsInit)
    {
        if (m_iServerFD > 0)
        {
            for (auto &pIter : m_hmConnection)
            {
                int iWrittenSize = pIter.second->WriteSocket(strContent);
                if (iWrittenSize > 0)
                {
                    SLOG2(slog::LL_DEBUG, "[TcpServer] Write socket %d total size %d", m_iServerFD, iWrittenSize);
                }
                else
                {
                    SLOG2(slog::LL_DEBUG, "[TcpServer] Write socket %d failed", m_iServerFD);
                }
            }
        }
    }
}

void TcpServerLauncher::Run()
{
    if (m_pServer)
    {
        m_pServer->Listen();
    }
}
} // namespace tcp