#pragma once
#include "core/base/Context.h"
#include <string>
#include <memory>

namespace tcp
{
class Connection;
class TcpContext : public core::base::Context
{
private:
    std::shared_ptr<Connection> m_pConnection;
    std::string m_strReadData;
    
public:
    TcpContext(const std::string &strReadData, std::shared_ptr<Connection> pConnection);
    ~TcpContext();

    std::shared_ptr<Connection> GetConnection();
    std::string GetData() const;
};
} // namespace tcp