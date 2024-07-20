/*
* File: TcpContext.cpp
* Class: TcpContext
* Created: 20211127
* Author: SonTV
*/
#include "tcp/TcpContext.h"
#include "tcp/Connection.h"

namespace tcp
{
TcpContext::TcpContext(TCPMessagePtr pMessage, ConnectionPtr pConnection) : core::base::TSContext()
, m_pConnection(pConnection), m_pMessage(pMessage)
{
}

TcpContext::~TcpContext()
{
}

ConnectionPtr TcpContext::GetConnection()
{
    return m_pConnection;
}

TCPMessagePtr TcpContext::GetMessage() const
{
    return m_pMessage;
}
} // namespace tcp