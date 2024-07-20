/*
* File: TCPMessage.h
* Class: TCPMessage
* Created: 20211128
* Author: SonTV
*/
#pragma once
#include <string>
#include <memory>

namespace tcp
{
class TCPMessage
{
private:
    char* m_szData;
    int m_iDataSize;
    int m_iRequestID;

public:
    TCPMessage(const int &iDataSize, char* pData, const int &iRqID);   
    virtual ~TCPMessage();

    int GetDataSize();
    void GetEncodeData(std::string &strEncodedData);
    char* GetData();
    int GetRequestID();
};
} // namespace tcp

typedef std::shared_ptr<tcp::TCPMessage> TCPMessagePtr;