/*
* File: SocketReadContext.h
* Class: SocketReadContext
* Created: 20211127
* Author: SonTV
*/
#pragma once
#include "core/base/Context.h"
#include <memory>
#include "Connection.h"

namespace tcp
{
class SocketReadContext: public core::base::TSContext
{
private:
    ConnectionPtr m_pConnection;

public:
    SocketReadContext(ConnectionPtr pConnection) : TSContext(), m_pConnection(pConnection)
    {
    }

    virtual ~SocketReadContext()
    {
    }
    
    ConnectionPtr GetConnection()
    {
        return m_pConnection;
    }
};
} // namespace tcp