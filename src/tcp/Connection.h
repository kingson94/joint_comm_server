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
#include "tcp/Message.h"

namespace core
{
namespace op
{
class Engine;
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
    core::op::Engine* m_pEngine;
    MessagePtr m_pMessage;

public:
    Connection(const int &iFD);
    ~Connection();
    int GetSocket();

    void PuskTaskReadSocket();
    void PuskTaskProcessService(MessagePtr pMessage);
    bool ReadSocket();
    int WriteSocket(MessagePtr pMessage);
}; 
} // namespace tcp

typedef std::shared_ptr<tcp::Connection> ConnectionPtr;


