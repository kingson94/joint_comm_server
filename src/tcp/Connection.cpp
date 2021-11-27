/*
* File: Connection.cpp
* Class: Connection
* Created: 20211127
* Author: SonTV
*/
#include "tcp/Connection.h"
#include <cstring>
#include "core/operator/Engine.h"
#include "AppManager.h"
#include "core/base/Task.h"
#include "service/TcpReadService.h"
#include "tcp/ReadContext.h"
#include "tcp/TcpContext.h"
#include <unistd.h>
#include "util/Utils.h"
#include <sys/socket.h>
#include "tcp/TcpServer.h"
#include "AppDefine.h"

namespace tcp
{
Connection::Connection(const int &iFD) : m_iFD(iFD)
{
    auto pApp = core::AppManager::GetInstance();
    if (pApp)
    {
        m_pTcpServer = (tcp::TcpServer*) pApp->GetComponent(TCP_SERVER_COMP).get();
        m_pEngine = (core::op::Engine*) pApp->GetComponent(ENGINE_COMP).get();
    }
}

Connection::~Connection()
{
}

void Connection::PuskTaskReadSocket()
{
    if (m_pEngine)
    {
        core::base::Context *pContext = new tcp::ReadContext(std::make_shared<Connection>(m_iFD));
        core::base::Task *pTask = new core::base::Task(TCP_READ_SERVICE_TYPE, pContext);
        m_pEngine->PushTask(pTask);
    }
}

int Connection::GetSocket()
{
    return m_iFD;
}

int Connection::WriteSocket(const std::string &strData)
{
    std::unique_lock<std::mutex> lckWriteSock(m_mtxWriteSocket);

    int iPacketSize = (int) (strData.size() + TCP_HEADER_SIZE);
    char* szPacket =  new char[iPacketSize];
    int iTotalSentByte = 0;
	int iTryTimes = WRITE_TRY_TIMES_MAX;

    if (!szPacket)
    {
        SLOG(slog::LL_DEBUG, "[Connection] Create packet failed");
        return -1;
    }

    // Encode data
    szPacket[0] = MAGIC_PACKET_BYTE;
    szPacket[1] = MAGIC_PACKET_BYTE;
    szPacket[2] = (iPacketSize >> 8) & 0xFF;
    szPacket[3] = iPacketSize & 0xFF;

    memcpy(szPacket + TCP_HEADER_SIZE, strData.c_str(), strData.size());

	while (iTotalSentByte < iPacketSize)
	{
        int iCurSentByte = write(m_iFD, szPacket, iPacketSize);
		if (iCurSentByte == -1)
		{
			if (errno == EAGAIN || errno == EWOULDBLOCK)
			{
				iTryTimes--;
				if (iTryTimes < 0)
				{
                    SLOG2(slog::LL_DEBUG, "[Connection] Tried times reach zero");
                    return -1;
				}
				usleep(100);
			}
			else
            {
                return -1;
            }
		}
		else
		{
			iTotalSentByte += iCurSentByte;
			iTryTimes = WRITE_TRY_TIMES_MAX;
		}
	}
    SAFE_DEL(szPacket);

    return iTotalSentByte;
}

void Connection::PuskTaskProcessService(const std::string &strReadData)
{	
    if (m_pEngine)
    {
        ConnectionPtr pConnection = m_pTcpServer->GetConnection(m_iFD);
        core::base::Context *pTcpContext = new TcpContext(strReadData, pConnection);
        core::base::Task *pTask = new core::base::Task(TCP_SERVICE_TYPE, pTcpContext);
        m_pEngine->PushTask(pTask);
    }
}

bool Connection::ReadSocket()
{
    int iReceived = 0;
    int nPacketSize = 0;
    char *szPayload = NULL;
    unsigned char szHeader[TCP_HEADER_SIZE];
    memset(szHeader, 0x00, sizeof(szHeader));

    while (true)
    {
        if (iReceived < TCP_HEADER_SIZE)
        {
            int iReadCount = read(m_iFD, szHeader, TCP_HEADER_SIZE - iReceived);
            if (iReadCount == 0)
            {
                SLOG2(slog::LL_DEBUG, "[Connection] Close connection %d", m_iFD);
                if (m_pTcpServer)
                {
                    m_pTcpServer->CloseConnection(m_iFD);
                }
                return true;
            }
            else if (iReadCount < 0)
            {
                if (errno != EAGAIN)
                {
                    SLOG2(slog::LL_DEBUG, "[Connection] Close connection %d", m_iFD);
                    if (m_pTcpServer)
                    {
                        m_pTcpServer->CloseConnection(m_iFD);
                    }
                    return true;
                }
                else
                {
                    continue;
                }
            }

            iReceived += iReadCount;

            if (iReceived == TCP_HEADER_SIZE)
            {
                if (szHeader[0] != MAGIC_PACKET_BYTE || szHeader[1] != MAGIC_PACKET_BYTE)
				{
					SLOG(slog::LL_DEBUG, "[Connection] Magic bytes are not matched");
					SLOG2(slog::LL_DEBUG, "[Connection] Close connection: %d", m_iFD);

					if (m_pTcpServer)
                    {
                        m_pTcpServer->CloseConnection(m_iFD);
                    }
					return true;
				}

                nPacketSize = (szHeader[2] << 8) + szHeader[3];
                if (nPacketSize > 0)
                {
                    szPayload = new char[nPacketSize - TCP_HEADER_SIZE];
                }
            }
        }
        else if (iReceived < nPacketSize)
        {
            int iReadCount = read(m_iFD, szPayload, nPacketSize - iReceived);
            if (iReadCount == 0)
            {
                SLOG2(slog::LL_DEBUG, "[Connection] Close connection %d", m_iFD);
                if (m_pTcpServer)
                {
                    m_pTcpServer->CloseConnection(m_iFD);
                }
                return true;
            }
            else if (iReadCount < 0)
            {
                if (errno != EAGAIN)
                {
                    SLOG2(slog::LL_DEBUG, "[Connection] Close connection %d", m_iFD);
                    if (m_pTcpServer)
                    {
                        m_pTcpServer->CloseConnection(m_iFD);
                    }
                    return true;
                }
                else
                {
                    continue;
                }
            }

            iReceived += iReadCount;

            if (iReceived == nPacketSize)
            {
                std::string strReadData = std::string(szPayload, nPacketSize - TCP_HEADER_SIZE);
                // SLOG2(slog::LL_DEBUG, "[Connection] Receive message from client: %s", strReadData.c_str());
                PuskTaskProcessService(strReadData);

                SAFE_DEL(szPayload);
                return true;
            }
        }
    }
    
    return true;
}
}