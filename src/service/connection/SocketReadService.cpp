/*
* File: SocketReadService.cpp
* Class: SocketReadService
* Created: 20211127
* Author: SonTV
*/
#include "service/ServiceDefine.h"
#include "tcp/SocketReadContext.h"
#include "tcp/Connection.h"
#include "util/Utils.h"

namespace service
{
bool SocketReadService::ProcessRequest(core::base::TSContext *pContext)
{
    tcp::SocketReadContext *pSocketReadContext = (tcp::SocketReadContext*) pContext;
    if (pSocketReadContext)
    {
        auto pConnection = pSocketReadContext->GetConnection();
        if (pConnection)
        {
            return pConnection->ReadSocket();
        }
    }
    return false;
}
}