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

void Message::EncodeData(std::string &strEncodedData)
{
    int iPacketSize = 0;
    char *pTmpPacket = new char[m_iDataSize + TCP_HEADER_SIZE];
    
    // Magic header
    pTmpPacket[0] = MAGIC_PACKET_BYTE;
    pTmpPacket[1] = MAGIC_PACKET_BYTE;

    // Data size
    pTmpPacket[2] = (m_iDataSize >> 24) & 0xFF;
    pTmpPacket[3] = (m_iDataSize >> 16) & 0xFF;
    pTmpPacket[4] = (m_iDataSize >> 8) & 0xFF;
    pTmpPacket[5] = m_iDataSize & 0xFF;

    // Request type
    pTmpPacket[6] = (m_iRequestType >> 8) & 0xFF;
    pTmpPacket[7] = m_iRequestType & 0xFF;
    iPacketSize += TCP_HEADER_SIZE;

    // Data
    memcpy(pTmpPacket + TCP_HEADER_SIZE, m_szData, m_iDataSize);
    iPacketSize += m_iDataSize;

    strEncodedData = std::string(pTmpPacket, iPacketSize);
    SAFE_DEL(pTmpPacket);
}

int Message::GetDataSize()
{
    return m_iDataSize;
}
} // namespace tcp
