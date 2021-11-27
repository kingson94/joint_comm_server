/*
* File: TcpReadService.cpp
* Class: TcpReadService
* Created: 20211127
* Author: SonTV
*/
#include "service/TcpReadService.h"
#include "tcp/ReadContext.h"
#include "tcp/Connection.h"
#include "util/Utils.h"

namespace service
{
bool TcpReadService::ProcessRequest(core::base::Context *pContext)
{
    tcp::ReadContext *pReadContext = (tcp::ReadContext*) pContext;
    if (pReadContext)
    {
        auto pConnection = pReadContext->GetConnection();
        if (pConnection)
        {
            return pConnection->ReadSocket();
        }
    }
    return false;
}
}