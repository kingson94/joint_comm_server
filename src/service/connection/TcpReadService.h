/*
* File: TcpReadService.h
* Class: TcpReadService
* Created: 20211127
* Author: SonTV
*/
#pragma once
#include "core/base/Service.h"
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
class TcpReadService : public core::base::Service
{
public:
    TcpReadService() : core::base::Service(TCP_READ_SERVICE_TYPE)
    {
    }

    ~TcpReadService()
    {
    }

    bool ProcessRequest(core::base::Context *pContext) override;
};
} // namespace service
