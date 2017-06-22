#ifndef CCLIENTNETWORK_H
#define CCLIENTNETWORK_H

#include <string>

#include <QDebug>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include <netinet/in.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/sendfile.h>

#include <stdio.h>

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

    bool ConnectToServer();
    bool Send(const char *message, int len);
    bool SendRequestCode(char cmd);
    bool SendFile(const char * fileName);
    bool ShutDown();
    ssize_t Recv(char* buff, int len);
    bool NonBlockingRecv(char *buf, int len, int timeToWait = 5);

private:
    bool InitSocket();
    bool InitSructAddr();
    bool InitClient();
    void initialize_flag();

private:
    std::string m_serverIpAddr;
    int m_port;
    int m_socket;

    struct sockaddr_in m_serverAddr;
};

#endif // CCLIENTNETWORK_H
