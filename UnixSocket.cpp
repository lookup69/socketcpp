#include "UnixSocket.h"

#include <fcntl.h>

using namespace lkup69;

UnixSocket::UnixSocket(int socket, int type, bool bConnected)
        : m_socket(socket),
          m_type(type),
          m_bConnected(bConnected)
{
}

UnixSocket::~UnixSocket()
{
        Close();
}

int UnixSocket::GetSocket()
{
        return m_socket;
}

void UnixSocket::Close()
{
        m_bConnected = false;

        if (m_socket != -1)
                close(m_socket);

        m_socket = -1;
}

// port would no used. compatiable with socket interface only
int UnixSocket::Bind(const std::string   &address,
                     [[maybe_unused]] int port)
{
        struct sockaddr_un unaddr {
                .sun_family = AF_UNIX,
                .sun_path
                {
                        0
                }
        };
        size_t len;

        assert(address.size() > 0);

        // Abstract Socket Namespace
        len = (address.size() > (sizeof(unaddr.sun_path) - 2)) ? (sizeof(unaddr.sun_path) - 2) : address.size();
        if (port == ABSTRACT_SOCKET) {
                memcpy(&unaddr.sun_path[1], address.c_str(), len);
        } else {
                unlink(address.c_str());
                memcpy(unaddr.sun_path, address.c_str(), len);
        }

        return bind(m_socket, (struct sockaddr *)&unaddr, sizeof(struct sockaddr_un));
}

int UnixSocket::Listen(int maxConnection)
{
        return listen(m_socket, maxConnection);
}

UnixSocket *UnixSocket::Accept()
{
        int sd;

        if ((sd = accept(m_socket, nullptr, 0)) == -1)
                return nullptr;

        return new (std::nothrow) UnixSocket{ sd, m_type };
}

int UnixSocket::Connect(const std::string &address)
{
        return this->Connect(address, 0);
}

int UnixSocket::Connect(const std::string &address, int port)
{
        struct sockaddr_un unaddr {
                .sun_family = 0,
                .sun_path
                {
                        0
                }
        };
        size_t len;

        assert(address.size() > 0);

        unaddr.sun_family = AF_UNIX;
        len               = (address.size() > (sizeof(unaddr.sun_path) - 2)) ? (sizeof(unaddr.sun_path) - 2) : address.size();

        if (port == ABSTRACT_SOCKET)
                memcpy(&unaddr.sun_path[1], address.c_str(), len);
        else
                memcpy(unaddr.sun_path, address.c_str(), len);

        if (connect(m_socket, (struct sockaddr *)&unaddr, sizeof(struct sockaddr_un)) == -1)
                return -1;

        m_bConnected = true;

        return 0;
}

ssize_t UnixSocket::Read(void *buf, size_t len)
{
        ssize_t ret;

        assert(buf);

        ret = read(m_socket, buf, len);
        if (ret < 0)
                m_bConnected = false;

        return ret;
}

ssize_t UnixSocket::Write(const void *buf, size_t len)
{
        ssize_t ret;

        assert(buf);

        ret = write(m_socket, buf, len);
        if (ret < 0)
                m_bConnected = false;

        return ret;
}

ssize_t UnixSocket::Write(const std::string &msg)
{
        ssize_t ret;
        size_t  len = msg.size();

        if (len == 0)
                return -1;

        ret = write(m_socket, msg.c_str(), len);
        if (ret < 0)
                m_bConnected = false;

        return ret;
}

ssize_t UnixSocket::Recv(void *buf, size_t len, int flags)
{
        ssize_t ret;

        ret = recv(m_socket, buf, len, flags);
        if (ret < 0)
                m_bConnected = false;

        return ret;
}

ssize_t UnixSocket::SendTo(const std::string   &msg,
                           const std::string   &destAddr,
                           [[maybe_unused]] int port,
                           [[maybe_unused]] int flags)
{
        struct sockaddr_un unaddr {
                .sun_family = AF_UNIX,
                .sun_path
                {
                        0
                }
        };

        int len = (destAddr.size() > (sizeof(unaddr.sun_path) - 2)) ? (sizeof(unaddr.sun_path) - 2) : destAddr.size();

        if (port == ABSTRACT_SOCKET)
                memcpy(&unaddr.sun_path[1], destAddr.c_str(), len);
        else
                memcpy(unaddr.sun_path, destAddr.c_str(), len);

        return sendto(m_socket, msg.c_str(), msg.size(), 0, (struct sockaddr *)&unaddr, len);
}

// static member function
UnixSocket *UnixSocket::CreateSocket(int type, bool bNonBlocking)
{
        int sd;

        // close socket when any of the exec-family functions succeed.
        type = type | SOCK_CLOEXEC;

        if (bNonBlocking)
                type = type | SOCK_NONBLOCK;

        if ((sd = socket(AF_UNIX, type, 0)) == -1)
                return nullptr;

        return new (std::nothrow) UnixSocket{ sd, type };
}
