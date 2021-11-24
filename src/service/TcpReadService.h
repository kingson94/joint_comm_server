#pragma once
#include "core/base/Service.h"
#define TCP_READ_SERVICE_TYPE 0
#define MAGIC_PACKET_BYTE 0x66

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
