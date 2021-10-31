/*
* File: TcpServer.cpp
* Class: TcpServer
* Created: 20211031
* Author: SonTV
*/

#include "TcpServer.h"
#include "utils/Define.h"
#include "ProtocolDefine.h"
namespace core
{
namespace protocol
{
    TcpServer::TcpServer() : core::Component(TCP_SERVER_COMP) 
    {
    }

    TcpServer::~TcpServer()
    {
    }

    void TcpServer::Run()
    {
        // Start TCP server
        std::printf("TCP run \n");
    }
}
}