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
    MessagePtr m_pMessage;
    
public:
    TcpContext(MessagePtr pMessage, ConnectionPtr pConnection);
    ~TcpContext();

    ConnectionPtr GetConnection();
    MessagePtr GetMessage() const;
};
} // namespace tcp