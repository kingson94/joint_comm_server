/*
* File: TcpServer.h
* Class: TcpServer
* Created: 20211024
* Author: SonTV
*/

#pragma once
#include "core/base/Component.h"
namespace core
{
namespace protocol
{
class TcpServer : public core::Component
{
public:
    TcpServer();
    ~TcpServer();
    void Run() override;
    void Init() override;
};
} // namespace protocol
} // namespace core