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
#include "Connection.h"

#define TCP_SERVER_COMP "TCP_SERVER_COMP"

namespace tcp
{
class TcpServerLauncher;
class TcpServer : public core::base::TSComponent
{
private:
    TcpServerLauncher *m_pLauncher;
    int m_iServerFD;
	int m_iEpollFD;

    std::string m_strHost;
    int m_iPort;

    boost::mutex m_mtxConnectionWrite;
    std::unordered_map<int, ConnectionPtr> m_hmConnection;
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
    void SendMessage(MessagePtr pMessage);

    void CloseConnection(const int &iFD);
    ConnectionPtr GetConnection(const int &iFD);
    void Join();
};

class TcpServerLauncher : public core::base::TSThread
{
private:
    TcpServer *m_pServer;

public:
    TcpServerLauncher(int iThreadID, TcpServer *pServer) : TSThread(iThreadID), m_pServer(pServer)
    {
    }

    virtual ~TcpServerLauncher()
    {
    }

    void Run();
};
} // namespace tcp