#include "TcpSocket.h"

#include <net/if.h>
#include <string.h>
#include <fcntl.h>

using namespace lkup69;

TcpSocket::TcpSocket(int socket, int domain)
        : m_socket(socket),
          m_domain(domain)
{
}

TcpSocket::~TcpSocket()
{
        Close();
}

int TcpSocket::GetSocket()
{
        return m_socket;
}

void TcpSocket::Close()
{
        if (m_socket != -1)
                close(m_socket);
        m_socket = -1;
}

int TcpSocket::Bind(int port)
{
        assert(m_socket != -1);

        m_inaddr.sin_family = m_domain;
        m_inaddr.sin_port   = htons(port);

        if (m_domain == AF_INET)
                m_inaddr.sin_addr.s_addr = htonl(INADDR_ANY);

        if (bind(m_socket, (struct sockaddr *)&m_inaddr, sizeof(m_inaddr)) == -1)
                return -1;

        return 0;
}

int TcpSocket::Bind(const std::string &address, int port)
{
        assert(m_socket != -1);

        m_inaddr.sin_family = m_domain;
        m_inaddr.sin_port   = htons(port);

        if (m_domain == AF_INET) {
                if (inet_aton(address.c_str(), &m_inaddr.sin_addr) == 0)
                        return -1;
        }

        if (bind(m_socket, (struct sockaddr *)&m_inaddr, sizeof(m_inaddr)) == -1)
                return -1;

        return 0;
}

int TcpSocket::Listen(int maxConnection)
{
        assert(m_socket != -1);

        return listen(m_socket, maxConnection);
}

Socket *TcpSocket::Accept()
{
        int sd = -1;
        socklen_t len = 0;

        assert(m_socket != -1);

        sd = accept(m_socket, (struct sockaddr *) &m_cliAddr, &len);

        if(sd < 0)
                return nullptr;


        return new TcpSocket{ sd, m_domain };
}

int TcpSocket::Connect(const std::string &address, int port)
{
        assert(m_socket != -1);

        m_inaddr.sin_family = m_domain;
        m_inaddr.sin_port   = htons(port);

        if (m_domain == AF_INET) {
                if (inet_aton(address.c_str(), &m_inaddr.sin_addr) == 0)
                        return -1;
        }

        if (connect(m_socket, (struct sockaddr *)&m_inaddr, sizeof(m_inaddr)) == -1)
                return -1;

        return 0;
}

ssize_t TcpSocket::Read(void *buf, size_t len)
{
        assert(buf);

        return read(m_socket, buf, len);
}

ssize_t TcpSocket::Write(const void *buf, size_t len)
{
        assert(buf);

        return write(m_socket, buf, len);
}

ssize_t TcpSocket::Write(const std::string &msg)
{
        size_t len = msg.size();

        assert(m_socket != -1);

        if (len == 0)
                return -1;

        return write(m_socket, msg.c_str(), len);
}

// static member function
TcpSocket *TcpSocket::CreateSocket(int domain, bool bNonBlocking)
{
        int sd;
        int type = SOCK_STREAM | SOCK_CLOEXEC;  // when any of the exec-family functions succeed.

        if (bNonBlocking)
                type = type | SOCK_NONBLOCK;

        if ((sd = socket(domain, type, 0)) == -1)
                return nullptr;

        const int on = 1;
        if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) != 0) {
                printf("[%s][%s][%d] setsockopt() ... fail\n", __FILE__, __PRETTY_FUNCTION__, __LINE__);
        }

        return new TcpSocket{ sd, domain };
}
