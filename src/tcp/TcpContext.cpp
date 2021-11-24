#include "tcp/TcpContext.h"
#include "tcp/Connection.h"

namespace tcp
{
TcpContext::TcpContext(const std::string &strReadData, std::shared_ptr<Connection> pConnection) : core::base::Context()
, m_pConnection(pConnection), m_strReadData(strReadData)
{
}

TcpContext::~TcpContext()
{
}

std::shared_ptr<Connection> TcpContext::GetConnection()
{
    return m_pConnection;
}

std::string TcpContext::GetData() const
{
    return m_strReadData;
}
} // namespace tcp