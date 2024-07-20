/*
* File: TcpContext.h
* Class: TcpContext
* Created: 20211127
* Author: SonTV
*/
#pragma once
#include "core/base/Context.h"
#include <string>
#include <memory>
#include "Connection.h"

namespace tcp
{
class TcpContext : public core::base::TSContext
{
private:
    ConnectionPtr m_pConnection;
    TCPMessagePtr m_pMessage;
    
public:
    TcpContext(TCPMessagePtr pMessage, ConnectionPtr pConnection);
    ~TcpContext();

    ConnectionPtr GetConnection();
    TCPMessagePtr GetMessage() const;
};
} // namespace tcp