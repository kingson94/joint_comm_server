#include "Profile.h"

namespace session
{
Profile::Profile()
{
}

Profile::Profile(const std::string &strAlias) : m_strAlias(strAlias)
{
}

Profile::~Profile()
{
}

std::string Profile::GetAlias() const
{
    return m_strAlias;
}

void Profile::SetAlias(const std::string &strAlias)
{
    if (m_strAlias != strAlias)
    {
        m_strAlias = strAlias;
    }
}

} // namespace session
