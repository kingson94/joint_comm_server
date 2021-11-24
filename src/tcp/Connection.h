#pragma once
#include <string>
#include <mutex>

#define TCP_HEADER_SIZE 4
#define WRITE_TRY_TIMES_MAX 10

namespace tcp
{
class TcpServer;
class Connection
{
private:
    // unsigned char szHeader[TCP_HEADER_SIZE];
    int m_iFD;
    std::mutex m_mtxWriteSocket;
    TcpServer* m_pTcpServer;

public:
    Connection(const int &iFD);
    ~Connection();
    int GetSocket();

    void PuskTaskReadSocket();
    // void PuskTaskProcessService(const std::string &strReadData);
    bool ReadSocket();
    int WriteSocket(const std::string &strData);
}; 
} // namespace tcp



