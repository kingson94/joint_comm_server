/*
* File: TcpService.cpp
* Class: TcpService
* Created: 20211127
* Author: SonTV
*/
#include "service/TcpService.h"
#include "tcp/TcpContext.h"
#include "tcp/Connection.h"
#include "tcp/Message.h"

namespace service
{
bool TcpService::ProcessRequest(core::base::Context *pContext)
{
    tcp::TcpContext *pTcpContext = (tcp::TcpContext*) pContext;
    auto pConnection = pTcpContext->GetConnection();
    MessagePtr pMessage = pTcpContext->GetMessage();

    if (pConnection)
    {
        // Response here if need
        // TSLOG2(tslog::LL_DEBUG, "content %s", strMsgContent.c_str());
        return true;
    }
    return false;
}
} // namespace service