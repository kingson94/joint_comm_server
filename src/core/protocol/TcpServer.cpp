/*
* File: TcpServer.cpp
* Class: TcpServer
* Created: 20211031
* Author: SonTV
*/

#include "TcpServer.h"
#include "utils/Define.h"
#include "ProtocolDefine.h"
#include "utils/Utils.h"
#include <sys/socket.h>

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
    if (m_bIsInit)
    {
        std::printf("TCP run %s ss\n", utils::Utils::GenerateUUID().c_str());
    }
}

void TcpServer::Init()
{
    Component::Init();

    // Load tcp config here
}
}
}