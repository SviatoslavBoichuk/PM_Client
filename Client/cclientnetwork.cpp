#include "cclientnetwork.h"
#include <QDebug>

CClientNetwork::CClientNetwork(const std::string &ipAddr, const int port) :
    m_serverIpAddr(ipAddr), m_port(port)
{
    initClient();
}

CClientNetwork::~CClientNetwork()
{
}

bool CClientNetwork::connectToServer()
{
    bool connected = false;

    if( connect(m_socket, (struct sockaddr *)&m_serverAddr, sizeof(m_serverAddr)) < 0)
    {
        connected = false;
    }
    else
    {
        connected = true;
    }

    return connected;
}

bool CClientNetwork::Send(const char *message, int len)
{
    send(m_socket, message, len, 0);
    return true;
}

bool CClientNetwork::shutDown()
{
    shutdown(m_socket, 0);
    return true; // !!!!!!
}

void CClientNetwork::Recv(char* buff, int len)
{
    recv(m_socket, buff, len, 0);
}

bool CClientNetwork::NonBlockingRecv(char *buf, int len, int timeToWait)
{
    bool readSuccess = false;

    fd_set read_s;
    struct timeval waitUntill;

    /* Watch stdin (fd 0) to see when it has input. */
    FD_ZERO(&read_s);
    FD_SET(m_socket, &read_s);
    /* Wait up to five seconds. */
    waitUntill.tv_sec = timeToWait;
    waitUntill.tv_usec = 0;
    int retVal = select(m_socket, &read_s, NULL, NULL, &waitUntill);

    /* Don’t rely on the value of tv now! */

    if (retVal == -1)
    {
        qDebug() << "select() error!";
    }
    else if (retVal)
    {
        qDebug() << "Data is available now.\n";
        FD_ISSET(m_socket, &read_s); // !!!
        Recv(buf, len);
        readSuccess = true;
    }
    else
    {
        qDebug() << "No data within five seconds.\n";
    }

    return readSuccess;
}

bool CClientNetwork::initSocket()
{
    bool initSuccess = false;

    if((m_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        initSuccess = false;
    }
    else
    {
        initSuccess = true;
    }

    return initSuccess;
}

bool CClientNetwork::initSructAddr()
{
    memset(&m_serverAddr, '0', sizeof(m_serverAddr));

    m_serverAddr.sin_family = AF_INET;
    m_serverAddr.sin_port = htons(m_port);
    m_serverAddr.sin_addr.s_addr = inet_addr(m_serverIpAddr.c_str());

    return true;
}

bool CClientNetwork::initClient()
{
    bool initSuccess = initSocket();

    if ( initSuccess )
    {
        initSuccess = initSructAddr();
    }


    return initSuccess;
}
