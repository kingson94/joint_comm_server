#pragma once

#include <glog/logging.h>

#ifndef _LOG_MONITOR_
#define _LOG_MONITOR_
#endif // _LOG_MONITOR_

#ifndef _LOG_FILE_
#define _LOG_FILE_
#endif // _LOG_FILE_

#ifdef _LOG_MONITOR_
#define LOG(level, pattern, ...) \
    DLOG(level) << 
#endif // _LOG_MONITOR_