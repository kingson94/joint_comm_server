/*
* File: Message.h
* Class: Message
* Created: 20211128
* Author: SonTV
*/
#pragma once
#include <string>
#include <memory>

namespace tcp
{
class Message
{
private:
    char* m_szData;
    int m_iDataSize;
    int m_iRequestType;

public:
    Message(const int &iDataSize, char* pData, const int &iRequestType);   
    ~Message();

    int GetDataSize();
    void GetEncodeData(std::string &strEncodedData);
    char* GetData();
};
} // namespace tcp

typedef std::shared_ptr<tcp::Message> MessagePtr;