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
    TSComponentPtr GetComponent(const std::string &strComponentID);
    void SendMessageToEndpoint(const std::string &strMessage);
    void SetRunningMode(const int &iMode);
    int GetRunningMode() const;

    void SetProfile(const std::string &strAlias = "");
    int RegisterService();

private:
    static AppManager* s_pIntance;
    std::unordered_map<std::string, TSComponentPtr> m_hmComponent;
    session::Profile m_obProfile;
    int m_iRunningMode;
};
} // namespace core