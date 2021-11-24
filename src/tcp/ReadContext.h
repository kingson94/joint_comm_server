#pragma once
#include "core/base/Context.h"
#include <memory>

namespace tcp
{
class Connection;
class ReadContext: public core::base::Context
{
private:
	std::shared_ptr<Connection> m_pConnection;

public:
	ReadContext(std::shared_ptr<Connection> pConnection) : Context(), m_pConnection(pConnection)
	{
	}

	virtual ~ReadContext()
	{
	}
    
	std::shared_ptr<Connection> GetConnection()
	{
		return m_pConnection;
	}
};
} // namespace tcp