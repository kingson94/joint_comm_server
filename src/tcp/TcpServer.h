/*
* File: TcpServer.h
* Class: TcpServer
* Created: 20211024
* Author: SonTV
*/

#pragma once
#include "core/base/Component.h"
#include "core/base/Thread.h"
#include <unordered_map>
#include <unordered_set>
#include <mutex>

#define TCP_SERVER_COMP "TCP_SERVER_COMP"

namespace tcp
{
class TcpServerLauncher;
class Connection;
class TcpServer : public core::base::Component
{
private:
    TcpServerLauncher *m_pLauncher;
    int m_iServerFD;
	int m_iEpollFD;

    std::string m_strHost;
    int m_iPort;

    std::mutex m_mtxConnectionWrite;
    std::unordered_map<int, std::shared_ptr<Connection>> m_hmConnection;
    std::unordered_set<int> m_setAcceptedFD;

private:
    int CreateBoundSocket(const std::string &strHost, const int &iPort);
    int OpenConnection(const std::string &strHost, const int &iPort);
    int MakeNonBlockingFD(const int &iFD);

public:
    TcpServer();
    ~TcpServer();
    void Run() override;
    void Init() override;
    void Listen();
    void SendMessage(const std::string &strContent);

    void CloseConnection(const int &iFD);
    void Join();
};

class TcpServerLauncher : public core::base::BaseThread
{
private:
    TcpServer *m_pServer;

public:
    TcpServerLauncher(int iThreadID, TcpServer *pServer) : BaseThread(iThreadID), m_pServer(pServer)
    {
    }

    virtual ~TcpServerLauncher()
    {
    }

    void Run();
};
} // namespace tcp