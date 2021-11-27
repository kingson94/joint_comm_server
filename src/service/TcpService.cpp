/*
* File: TcpService.cpp
* Class: TcpService
* Created: 20211127
* Author: SonTV
*/
#include "service/TcpService.h"
#include "tcp/TcpContext.h"
#include "tcp/Connection.h"

namespace service
{
bool TcpService::ProcessRequest(core::base::Context *pContext)
{
    // tcp::TcpContext *pTcpContext = (tcp::TcpContext*) pContext;
    // auto pConnection = pTcpContext->GetConnection();
    // std::string strMsgContent = pTcpContext->GetData();

    // if (pConnection)
    // {
    //     SLOG2(slog::LL_DEBUG, "Receive message from client: %s", strMsgContent.c_str());
    //     // Response here if need
    //     return true;
    // }
    return true;
}
} // namespace service