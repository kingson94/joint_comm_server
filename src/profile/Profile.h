#pragma once
#include <string>

namespace session
{
class Profile
{
private:
    std::string m_strAlias;

public:
    Profile();
    Profile(const std::string &strAlias);
    ~Profile();

    std::string GetAlias() const;
    void SetAlias(const std::string &strAlias);
};
} // namespace session