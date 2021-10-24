#pragma once
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
        