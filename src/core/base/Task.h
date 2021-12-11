/*
* File: Task.h
* Class: TSTask
* Created: 20211127
* Author: SonTV
*/
#pragma once
#include "core/base/Context.h"
#include "AppDefine.h"

namespace core
{
namespace base
{
class TSTask
{
private:
    int m_iType;
    TSContext *m_pContext;

public:
    TSTask(const int &iType, TSContext *pContext) : m_iType(iType), m_pContext(pContext)
    {
    }

    ~TSTask()
    {
        SAFE_DEL(m_pContext);
    }

    int GetType()
    {
        return m_iType;
    }

    TSContext* GetContext()
    {
        return m_pContext;
    }
};
} // namespace base
} // namespace core