/*
* File: Service.h
* Class: Service
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
class Service
{
protected:
	int m_iType;

public:
	Service(const int &iType): m_iType(iType)
	{
	}

	virtual ~Service()
	{
	}

    int GetType()
	{
		return m_iType;
	}

	virtual bool ProcessRequest(Context *pContext) = 0;
};
}  // namespace base
}  // namespace core

typedef std::shared_ptr<core::base::Service> ServicePtr;