/*
 * File: AppManager.h
 * Class: AppManager
 * Created: 20211024
 * Author: SonTV
*/

#pragma once
#include <unordered_map>
#include "core/base/Component.h"
#include "profile/Profile.h"
#include "util/Utils.h"

#define RUNNING_MODE_SERVER 0
#define RUNNING_MODE_CLIENT 1
namespace core
{
class AppManager
{
public:
    static AppManager* GetInstance();
    static int CreateInstance();
    static int DestroyInstance();
    void Run();
    void Init();

    ~AppManager();

    // Register comp
    void RegisterComponents();
    core::base::Component* GetComponent(const std::string &strComponentID);
    void SendMessageToEndpoint(const std::string &strMessage);
    void SetRunningMode(const int &iMode);
    int GetRunningMode() const;

    void SetProfile(const std::string &strAlias = "");

private:
    static AppManager* s_pIntance;
    std::unordered_map<std::string, core::base::Component*> m_hmComponent;
    session::Profile m_obProfile;
    int m_iRunningMode;
};
} // namespace core