/*
* File: Utils.h
* Class: Utils
* Created: 20211107
* Author: SonTV
*/
#pragma once
#include <string>
#include <uuid/uuid.h>

namespace utils
{
class Utils
{
public:
    Utils();
    ~Utils();

    static inline std::string GenerateUUID()
    {
        char arrTmp[40];
        uuid_t arrUUID;
        std::string strResult = "";

        uuid_generate_time(arrUUID);
        uuid_unparse_lower(arrUUID, arrTmp);
        strResult.assign(arrTmp);
        return strResult;
    }
};
} // namespace utils
