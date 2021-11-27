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
TcpContext::TcpContext(const std::string &strReadData, ConnectionPtr pConnection) : core::base::Context()
, m_pConnection(pConnection), m_strReadData(strReadData)
{
}

TcpContext::~TcpContext()
{
}

ConnectionPtr TcpContext::GetConnection()
{
    return m_pConnection;
}

std::string TcpContext::GetData() const
{
    return m_strReadData;
}
} // namespace tcp