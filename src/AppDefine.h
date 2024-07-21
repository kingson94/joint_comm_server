/*
* File: AppDefine.h
* Created: 20211107
* Author: SonTV
*/
#pragma once

#include <string>

#define TCP_SERVER_CONFIG_FILE "config/tcp_server.json"
#define TCP_CLIENT_CONFIG_FILE "config/tcp_client.json"
#define ENGINE_CONFIG_FILE "config/engine.json"
#define SERVICE_CONFIG_FILE "config/service.json"

// Engine serivces
#define SOCKET_READ_SERVICE_ID 0

// Tcp
#define DEFAULT_TCP_PORT 8012
#define MAGIC_PACKET_BYTE 0x66
#define TCP_HEADER_SIZE 8
#define WRITE_TRY_TIMES_MAX 10

// App
#define RUNNING_MODE_SERVER 0

// Engine
#define DEFAULT_TASK_QUEUE_MAX_SIZE 1000
#define DEFAULT_WORKER_COUNT 8 

// Component
#define ENGINE_COMP "ENGINE_COMP"
#define TCP_SERVER_COMP "TCP_SERVER_COMP"

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
