/*
 * File: AppManager.h
 * Class: AppManager
 * Created: 20211024
 * Author: SonTV
*/

#pragma once
#include <unordered_map>
#include "core/base/Component.h"

namespace core
{
class AppManager
{
public:
    static AppManager* GetInstance();
    static int CreateInstance();
    static int DestroyInstance();
    void Run();

    // Register comp
    void RegisterComponents();

private:
    static AppManager* s_pIntance;
    std::unordered_map<std::string, core::Component*> m_hmComponent;
};
} // namespace core