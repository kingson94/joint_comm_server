/*
* File: HttpServer.h
* Class: HttpServer
* Created: 20211222
* Author: SonTV
*/
#pragma once
#include "HttpContext.h"
#include "core/base/Component.h"
#include "core/base/Thread.h"
#include "nghttp2/asio_http2_server.h"
#include "log/LogDefine.h"

namespace http2 = nghttp2::asio_http2;
namespace http2_server = nghttp2::asio_http2::server;

namespace http
{
class HttpServerLauncher;
class HttpServer : public core::base::TSComponent
{
private:
    HttpServerLauncher *m_pLauncher;
    http2_server::http2 m_obHttp2;

public:
    HttpServer();
    ~HttpServer();

    void Run() override;
    void Init() override;

    void Listen();
    void ProcessRequest(const std::string &strData);
};

class HttpServerLauncher : public core::base::TSThread
{
private:
    HttpServer *m_pServer;

public:
    HttpServerLauncher(int iThreadID, HttpServer *pServer) : TSThread(iThreadID), m_pServer(pServer)
    {
    }

    virtual ~HttpServerLauncher()
    {
    }

    void Run();
};
} // namespace http