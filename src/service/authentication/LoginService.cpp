/*
* File: LoginService.cpp
* Class: LoginService
* Created: 20211210
* Author: SonTV
*/
#include "service/ServiceDefine.h"

namespace service
{
bool service::LoginService::ProcessRequest(core::base::TSContext *pContext)
{
    // tcp::SocketReadContext *pSocketReadContext = (tcp::SocketReadContext*) pContext;
    // if (pSocketReadContext)
    // {
    //     auto pConnection = pSocketReadContext->GetConnection();
    //     if (pConnection)
    //     {
    //         return pConnection->ReadSocket();
    //     }
    // }
    return false;
}
}