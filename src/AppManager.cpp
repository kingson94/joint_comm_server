/*
* File: AppManager.cpp
* Class: AppManager
* Created: 20211024
* Author: SonTV
*/

#include "AppManager.h"
#include <utils/Define.h>

namespace core
{
AppManager* AppManager::s_pIntance = NULL;
AppManager* AppManager::GetInstance()
{
    return s_pIntance;
}

int AppManager::CreateInstance()
{
    if (!s_pIntance)
    {
        s_pIntance = new AppManager();
        return 0;
    }

    return 1;
}

int AppManager::DestroyInstance()
{
    SAFE_DEL(s_pIntance);
    return 0;
}
}