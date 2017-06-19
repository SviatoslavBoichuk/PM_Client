#ifndef CCLIENTNETWORK_H
#define CCLIENTNETWORK_H

#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <unistd.h>

#include "pm_packets.h"

/******************************************************/
const std::string g_cDefaultIp = "127.0.0.1"; // 127.0.0.1
const std::string g_cServerIp = "172.20.8.132";
const int g_ciDefaultPort  = 80;
const int g_ciServerPort = 7890;
/*******************************************************/


class CClientNetwork
{
public:
    explicit CClientNetwork(const std::string& ipAddr, const int port);
    CClientNetwork(const CClientNetwork&)               = delete;
    CClientNetwork& operator =(const CClientNetwork&)   = delete;
    ~CClientNetwork();

    bool connectToServer();
    bool Send(const char *message, int len);
    bool shutDown();
    void Recv(char* buff, int len);
    bool NonBlockingRecv(char *buf, int len, int timeToWait = 5);

private:
    bool initSocket();
    bool initSructAddr();
    bool initClient();

private:
    std::string m_serverIpAddr;
    int m_port;
    int m_socket;

    struct sockaddr_in m_serverAddr;
};

#endif // CCLIENTNETWORK_H
