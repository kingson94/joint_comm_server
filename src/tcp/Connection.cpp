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
#include "service/connection/TcpReadService.h"
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

int Connection::WriteSocket(MessagePtr pMessage)
{
    boost::mutex::scoped_lock lckWriteSock(m_mtxWriteSocket);
    if (!pMessage)
    {
        return -1;
    }

    int iTotalSentByte = 0;
	int iTryTimes = WRITE_TRY_TIMES_MAX;
    std::string strEncodeData = "";
    int iEncodeSize = 0;

    pMessage->EncodeData(strEncodeData);
    iEncodeSize = strEncodeData.size();

    if (iEncodeSize < 0)
    {
        TSLOG(tslog::LL_DEBUG, "[Connection] Create packet failed");
        return -1;
    }

	while (iTotalSentByte < iEncodeSize)
	{
        int iCurSentByte = write(m_iFD, strEncodeData.c_str(), iEncodeSize);
		if (iCurSentByte == -1)
		{
			if (errno == EAGAIN || errno == EWOULDBLOCK)
			{
				iTryTimes--;
				if (iTryTimes < 0)
				{
                    TSLOG2(tslog::LL_DEBUG, "[Connection] Tried times reach zero");
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

    return iTotalSentByte;
}

void Connection::PuskTaskProcessService(MessagePtr pMessage)
{	
    if (m_pEngine)
    {
        ConnectionPtr pConnection = m_pTcpServer->GetConnection(m_iFD);
        core::base::Context *pTcpContext = new TcpContext(pMessage, pConnection);
        core::base::Task *pTask = new core::base::Task(TCP_SERVICE_TYPE, pTcpContext);
        m_pEngine->PushTask(pTask);
    }
}

bool Connection::ReadSocket()
{
    int iReceived = 0;
    int nPacketSize = 0;
    int iRequestType = -1;
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
                TSLOG2(tslog::LL_DEBUG, "[Connection] Close connection %d", m_iFD);
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
                    TSLOG2(tslog::LL_DEBUG, "[Connection] Close connection %d", m_iFD);
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
					TSLOG(tslog::LL_DEBUG, "[Connection] Magic bytes are not matched");
					TSLOG2(tslog::LL_DEBUG, "[Connection] Close connection: %d", m_iFD);

					if (m_pTcpServer)
                    {
                        m_pTcpServer->CloseConnection(m_iFD);
                    }
					return true;
				}

                nPacketSize += (szHeader[2] << 24);
                nPacketSize += (szHeader[3] << 16);
                nPacketSize += (szHeader[4] << 8);
                nPacketSize += szHeader[5];
                if (nPacketSize > 0)
                {
                    szPayload = new char[nPacketSize - TCP_HEADER_SIZE];
                }
                
                iRequestType += (szHeader[6] << 8);
                iRequestType += szHeader[7];
            }
        }
        else if (iReceived < nPacketSize)
        {
            int iReadCount = read(m_iFD, szPayload, nPacketSize - iReceived);
            if (iReadCount == 0)
            {
                TSLOG2(tslog::LL_DEBUG, "[Connection] Close connection %d", m_iFD);
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
                    TSLOG2(tslog::LL_DEBUG, "[Connection] Close connection %d", m_iFD);
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
                MessagePtr pMessage = std::make_shared<tcp::Message>(nPacketSize - TCP_HEADER_SIZE, szPayload, iRequestType);
                PuskTaskProcessService(pMessage);
                return true;
            }
        }
    }
    
    return true;
}
}