/*
* File: TcpService.h
* Class: TcpService
* Created: 20211127
* Author: SonTV
*/
#pragma once
#include "core/base/Service.h"
#include "util/Utils.h"
#include "AppDefine.h"

namespace core
{
namespace base
{
class Context;
}
}
namespace service
{
class TcpService : public core::base::Service
{
public:
    TcpService() : core::base::Service(TCP_SERVICE_TYPE)
    {
    }

    ~TcpService()
    {
    }

    bool ProcessRequest(core::base::Context *pContext) override;
};
} // namespace service
