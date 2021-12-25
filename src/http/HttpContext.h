/*
* File: HttpContext.h
* Class: HttpContext
* Created: 20211222
* Author: SonTV
*/
#pragma once
#include "core/base/Context.h"
#include <string>
#include <memory>

namespace http
{
class HttpContext : public core::base::TSContext
{
private:
    // uWS::HttpResponse  m_stResponse;
    // uWS::HttpRequest m_stRequest;
    
public:
    // HttpContext(const uWS::HttpRequest &stHttRequest, const uWS::HttpResponse &stHttpResponse);
    ~HttpContext();

    // uWS::HttpRequest &GetRequest();
    // uWS::HttpResponse &GetResponse();
};
} // namespace http

typedef std::shared_ptr<http::HttpContext> HttpContextPtr;