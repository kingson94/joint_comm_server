/*
* File: Service.h
* Class: TSService
* Created: 20211127
* Author: SonTV
*/
#pragma once
#include "Context.h"
#include <memory>

namespace core
{
namespace base
{
class TSService
{
protected:
    int m_iID;

public:
    TSService()
    {
    }

    virtual ~TSService()
    {
    }

    int GetID()
    {
        return m_iID;
    }

    void SetID(const int &iID)
    {
        m_iID = iID;
    }

    virtual bool ProcessRequest(TSContext *pContext) = 0;
};
}  // namespace base
}  // namespace core

typedef std::shared_ptr<core::base::TSService> TSServicePtr;