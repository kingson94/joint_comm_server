/*
* File: ReadContext.h
* Class: ReadContext
* Created: 20211127
* Author: SonTV
*/
#pragma once
#include "core/base/Context.h"
#include <memory>
#include "Connection.h"

namespace tcp
{
class ReadContext: public core::base::Context
{
private:
	ConnectionPtr m_pConnection;

public:
	ReadContext(ConnectionPtr pConnection) : Context(), m_pConnection(pConnection)
	{
	}

	virtual ~ReadContext()
	{
	}
    
	ConnectionPtr GetConnection()
	{
		return m_pConnection;
	}
};
} // namespace tcp