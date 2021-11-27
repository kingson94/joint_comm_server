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
class TcpContext : public core::base::Context
{
private:
    ConnectionPtr m_pConnection;
    std::string m_strReadData;
    
public:
    TcpContext(const std::string &strReadData, ConnectionPtr pConnection);
    ~TcpContext();

    ConnectionPtr GetConnection();
    std::string GetData() const;
};
} // namespace tcp