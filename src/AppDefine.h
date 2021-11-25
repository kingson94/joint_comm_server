/*
* File: Define.h
* Created: 20211107
* Author: SonTV
*/
#pragma once

#include <string>

#define TCP_SERVER_CONFIG_FILE "config/tcp_server.json"
#define TCP_CLIENT_CONFIG_FILE "config/tcp_client.json"
#define ENGINE_CONFIG_FILE "config/engine.json"

// Tcp
#define DEFAULT_TCP_PORT 8012

// App
#define RUNNING_MODE_SERVER 0
#define RUNNING_MODE_CLIENT 1

// Engine
#define DEFAULT_QUEUE_SIZE 1000
#define DEFAULT_WORKER_COUNT 8 

// Memory
#ifndef SAFE_DEL
#define SAFE_DEL(x) \
        if (x) \
        { \
            delete x; \
            x = NULL; \
        }
#endif // SAFE_DEL

#ifndef NULL
#define NULL 0
#endif // NULL
