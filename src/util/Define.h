#pragma once

#include <string>

#define TCP_SERVER_CONFIG_FILE "config/tcp_server.json"
#define TCP_CLIENT_CONFIG_FILE "config/tcp_client.json"
#define ENGINE_CONFIG_FILE "config/engine.json"

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