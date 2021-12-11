/*
* File: ServiceDefine.h
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
class TSContext;
}
}

#define DECLARE_SERVICE(strServiceName) \
namespace service \
{ \
class strServiceName##Service : public core::base::TSService \
{ \
public: \
    strServiceName##Service() : core::base::TSService() \
    { \
    } \
    \
    ~strServiceName##Service() \
    { \
    } \
    \
    bool ProcessRequest(core::base::TSContext *pContext) override; \
}; \
}

DECLARE_SERVICE(SocketRead)
DECLARE_SERVICE(Authenticate)
DECLARE_SERVICE(Login)
