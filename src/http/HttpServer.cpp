/*
* File: HttpServer.cpp
* Class: HttpServer
* Created: 20211222
* Author: SonTV
*/

#include "AppDefine.h"
#include "HttpServer.h"

namespace http
{
HttpServer::HttpServer() : core::base::TSComponent(HTTP_SERVER_COMP)
{
}

HttpServer::~HttpServer()
{
}

void HttpServer::ProcessRequest(const std::string &strData)
{
}

void HttpServer::Run()
{
    m_pLauncher = new HttpServerLauncher(102, this);
    m_pLauncher->Start();
}

void HttpServer::Listen()
{
    boost::system::error_code obError;
    m_obHttp2.handle("/", [](const http2_server::request &obRequest, const http2_server::response &obResponse)
    {
        obResponse.write_head(200);
        obResponse.end("hello");
        TSLOG(tslog::LL_DEBUG, "Hello");
    });
    
    m_obHttp2.listen_and_serve(obError, "0.0.0.0", "8011");
    m_obHttp2.join();
}

void HttpServer::Init()
{

}

void HttpServerLauncher::Run()
{
   if (m_pServer)
   {
       m_pServer->Listen();
   }
}
} // namespace http
