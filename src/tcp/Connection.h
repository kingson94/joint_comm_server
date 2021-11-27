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

#define TCP_HEADER_SIZE 4
#define WRITE_TRY_TIMES_MAX 10

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

public:
    Connection(const int &iFD);
    ~Connection();
    int GetSocket();

    void PuskTaskReadSocket();
    void PuskTaskProcessService(const std::string &strReadData);
    bool ReadSocket();
    int WriteSocket(const std::string &strData);

}; 
} // namespace tcp

typedef std::shared_ptr<tcp::Connection> ConnectionPtr;


