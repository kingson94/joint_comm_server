/*
* File: Connection.h
* Class: Connection
* Created: 20211127
* Author: SonTV
*/
#pragma once
#include <string>
#include "boost/thread/mutex.hpp"
#include <memory>
#include "tcp/TCPMessage.h"

namespace core
{
namespace op
{
class TSEngine;
} // namespace base
} // namespace core


namespace tcp
{
class TcpServer;
class Connection
{
private:
    int m_iFD;
    boost::mutex m_mtxWriteSocket;
    TcpServer* m_pTcpServer;
    core::op::TSEngine* m_pEngine;
    TCPMessagePtr m_pMessage;

public:
    Connection(const int &iFD);
    ~Connection();
    int GetSocket();

    void PuskTaskReadSocket();
    void PuskTaskProcessService(TCPMessagePtr pMessage);
    bool ReadSocket();
    int WriteSocket(TCPMessagePtr pMessage);
}; 
} // namespace tcp

typedef std::shared_ptr<tcp::Connection> ConnectionPtr;


