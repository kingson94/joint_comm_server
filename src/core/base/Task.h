#pragma once
#include "core/base/Context.h"
namespace core
{
namespace base
{
class Task
{
private:
    int m_iType;
    Context *m_pContext;

public:
    Task(const int &iType, Context *pContext) : m_iType(iType), m_pContext(pContext)
    {
    }

    ~Task()
    {
    }

    int GetType()
    {
        return m_iType;
    }

    Context* GetContext()
    {
        return m_pContext;
    }
};
} // namespace base
} // namespace core