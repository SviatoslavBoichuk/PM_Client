#include "cclientnetwork.h"


CClientNetwork::CClientNetwork(const std::string &ipAddr, const int port) :
    m_serverIpAddr(ipAddr), m_port(port)
{
    InitClient();
}

CClientNetwork::~CClientNetwork()
{
    // break communication
    ShutDown();
}

bool CClientNetwork::ConnectToServer()
{
    bool connected = false;

    if( connect(m_socket, (struct sockaddr *)&m_serverAddr, sizeof(m_serverAddr)) != 0)
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

bool CClientNetwork::SendRequestCode(char cmd)
{
    return Send( (const char*)&cmd, 1 );
}

bool CClientNetwork::SendFile(const char *fileName)
{
    int fd;
    int sent_bytes = 0;
    struct stat file_stat;
    int offset = 0;
    int remain_data;

    fd = open(fileName, O_RDONLY);

    if (fd == -1)
    {
        qDebug() << "Error opening file -->";
        return false;
    }

    if (fstat(fd, &file_stat) < 0)
    {
        qDebug() << "Error fstat -->";
        return false;
    }

    remain_data = file_stat.st_size;

    while (((sent_bytes = sendfile(m_socket, fd, (off_t*)&offset, BUFSIZ)) > 0) && (remain_data > 0))
    {
        remain_data -= sent_bytes;
        qDebug() << "send bytes: " << sent_bytes;
    }

    return true;
}

bool CClientNetwork::ShutDown()
{
    return (0 == shutdown(m_socket, SHUT_RDWR)) ? true : false;
}

ssize_t CClientNetwork::Recv(char* buff, int len)
{
    return recv(m_socket, buff, len, 0);
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
    int retVal = select(m_socket +1 , &read_s, NULL, NULL, &waitUntill);

    /* Don’t rely on the value of tv now! */
    if (retVal == -1)
    {
        qDebug() << "select() error!";
    }
    else if (retVal)
    {
        FD_ISSET(m_socket, &read_s); // !!!
        qDebug() << "buf ch";
        Recv(buf, len);
        readSuccess = true;
        qDebug() << "ad";
    }
    else
    {
        ; /* !!! */
    }

    return readSuccess;
}

bool CClientNetwork::InitSocket()
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

bool CClientNetwork::InitSructAddr()
{
    memset(&m_serverAddr, 0, sizeof(m_serverAddr));

    m_serverAddr.sin_family = AF_INET;
    m_serverAddr.sin_port = htons(m_port);
    m_serverAddr.sin_addr.s_addr = inet_addr(m_serverIpAddr.c_str());

    return true;
}

bool CClientNetwork::InitClient()
{
    bool initSuccess = InitSocket();

    if ( initSuccess )
    {
        initSuccess = InitSructAddr();
    }


    return initSuccess;
}
