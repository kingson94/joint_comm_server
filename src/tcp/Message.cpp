/*
* File: Message.cpp
* Class: Message
* Created: 20211128
* Author: SonTV
*/
#include "Message.h"
#include "AppDefine.h"
#include <cstring>

// Magigc 2B|Size 4B|Type 2B|Data
namespace tcp
{
Message::Message(const int &iSize, char* pData, const int &iType) 
: m_iDataSize(iSize), m_szData(pData), m_iRequestType(iType)
{
}

Message::~Message()
{
    SAFE_DEL(m_szData);
}

void Message::GetEncodeData(std::string &strEncodedData)
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

    // Request type
    pTmpPacket[6] = (m_iRequestType >> 8) & 0xFF;
    pTmpPacket[7] = m_iRequestType & 0xFF;

    // Data
    memcpy(pTmpPacket + TCP_HEADER_SIZE, m_szData, m_iDataSize);

    strEncodedData = std::string(pTmpPacket, iPacketSize);
    SAFE_DEL(pTmpPacket);
}

int Message::GetDataSize()
{
    return m_iDataSize;
}

char* Message::GetData()
{
    return m_szData;
}
} // namespace tcp
