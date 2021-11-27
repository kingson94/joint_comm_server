/*
* File: TcpClient.h
* Class: TcpClient
* Created: 20211024
* Author: SonTV
*/
#pragma once
#include "core/base/Component.h"
#include "core/base/Thread.h"
#include <unordered_map>
#include <mutex>
#include <tcp/Connection.h>

namespace tcp
{
class TcpClientLauncher;
class TcpClient : public core::base::Component
{
private:
    TcpClientLauncher *m_pLauncher;
    int m_iClientFD;

    std::string m_strServerHost;
    int m_iServerPort;

    std::mutex m_mtxConnectionWrite;
    std::unordered_map<int, ConnectionPtr> m_hmConnection;

private:
    int CreateBoundSocket(const std::string &strHost, const int &iPort);
    int OpenConnection(const std::string &strHost, const int &iPort);
    int MakeNonBlockingFD(const int &iFD);

public:
    TcpClient();
    ~TcpClient();
    void Run() override;
    void Init() override;
    bool Connect(const std::string &strHost, const int &iPort);
    void Listen();
    void SendMessage(const std::string &strContent);
    void AttachAuthortoMessage(const std::string &strAuthorName, const std::string &strMessageIn, std::string &strMessageOut);
};

class TcpClientLauncher : public core::base::BaseThread
{
private:
    TcpClient *m_pClient;

public:
    TcpClientLauncher(int iThreadID, TcpClient *pClient) : BaseThread(iThreadID), m_pClient(pClient)
    {
    }

    virtual ~TcpClientLauncher()
    {
    }

    void Run();
};
} // namespace tcp