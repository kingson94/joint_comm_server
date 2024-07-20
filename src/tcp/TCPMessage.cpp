/*
* File: TCPMessage.cpp
* Class: TCPMessage
* Created: 20211128
* Author: SonTV
*/
#include "TCPMessage.h"
#include "AppDefine.h"
#include <cstring>

// Magigc 2B|Size 4B|ID 2B|Data
namespace tcp
{
TCPMessage::TCPMessage(const int &iSize, char* pData, const int &iRqID) 
: m_iDataSize(iSize), m_szData(pData), m_iRequestID(iRqID)
{
}

TCPMessage::~TCPMessage()
{
    SAFE_DEL(m_szData);
}

void TCPMessage::GetEncodeData(std::string &strEncodedData)
{
    int iPacketSize = m_iDataSize + TCP_HEADER_SIZE;
    char *pTmpPacket = new char[iPacketSize];
    
    // Magic header
    pTmpPacket[0] = MAGIC_PACKET_BYTE;
    pTmpPacket[1] = MAGIC_PACKET_BYTE;

    // Data size
    pTmpPacket[2] = (iPacketSize >> 24) & 0xFF;
    pTmpPacket[3] = (iPacketSize >> 16) & 0xFF;
    pTmpPacket[4] = (iPacketSize >> 8) & 0xFF;
    pTmpPacket[5] = iPacketSize & 0xFF;

    // Request id
    pTmpPacket[6] = (m_iRequestID >> 8) & 0xFF;
    pTmpPacket[7] = m_iRequestID & 0xFF;

    // Data
    memcpy(pTmpPacket + TCP_HEADER_SIZE, m_szData, m_iDataSize);

    strEncodedData = std::string(pTmpPacket, iPacketSize);
    SAFE_DEL(pTmpPacket);
}

int TCPMessage::GetDataSize()
{
    return m_iDataSize;
}

char* TCPMessage::GetData()
{
    return m_szData;
}

int TCPMessage::GetRequestID()
{
    return m_iRequestID;
}
} // namespace tcp
