/*
 * File: AppManager.h
 * Class: AppManager
 * Created: 20211024
 * Author: SonTV
*/
#pragma once

namespace core
{
class AppManager
{
public:
    static AppManager* GetInstance();

private:
    static AppManager* s_pIntance;
    int CreateInstance();
    int DestroyInstance();
};
} // namespace core