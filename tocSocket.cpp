#include "unixSocket.h"

using namespace lkup69;

TcpSocket::UnixSocket(int socket)
        : m_socket(socket)
{
}

UnixSocket::~UnixSocket()
{
        if (m_socket != -1)
                close(m_socket);
        m_socket = -1;
}

int UnixSocket::GetSocket()
{
        return m_socket;
}

void UnixSocket::Close()
{
        if (m_socket != -1)
                close(m_socket);
        m_socket = -1;
}

int UnixSocket::Bind(const std::string &address)
{
        struct sockaddr_un unaddr = { 0 };
        size_t             len;

        assert(address.size() > 0);

        unaddr.sun_family = AF_UNIX;

        // Abstract Socket Namespace
        len = (address.size() > (sizeof(unaddr.sun_path) - 2)) ? (sizeof(unaddr.sun_path) - 2) : address.size();
        memcpy(unaddr.sun_path, address.c_str(), len);

        if (bind(m_socket, (struct sockaddr *)&unaddr, sizeof(struct sockaddr_un)) == -1)
                return -1;

        return 0;
}

int UnixSocket::Listen(int maxConnection)
{
        return listen(m_socket, maxConnection);
}

UnixSocket *UnixSocket::Accept()
{
        int sd;

        if ((sd = accept(m_socket, NULL, 0)) == -1) 
                return nullptr;

        return new UnixSocket{ sd };
}

int UnixSocket::Connect(const std::string &address)
{
        struct sockaddr_un unaddr = { 0 };
        size_t             len;

        assert(address.size() > 0);

        unaddr.sun_family = AF_UNIX;
        len               = (address.size() > (sizeof(unaddr.sun_path) - 2)) ? (sizeof(unaddr.sun_path) - 2) : address.size();
        memcpy(unaddr.sun_path, address.c_str(), len);

        if (connect(m_socket, (struct sockaddr *)&unaddr, sizeof(struct sockaddr_un)) == -1)
                return -1;

        return 0;
}

int UnixSocket::Read(void *buf, size_t len)
{
        assert(buf);

        return read(m_socket, buf, len);
}

int UnixSocket::Write(const void *buf, size_t len)
{
        assert(buf);

        return write(m_socket, buf, len);
}

int UnixSocket::Write(const std::string &msg)
{
        size_t len = msg.size();

        if (len == 0)
                return -1;

        return write(m_socket, msg.c_str(), len);
}

// static member function
UnixSocket *UnixSocket::CreateSocket(int type)
{
        int sd;

        if ((sd = socket(AF_UNIX, type, 0)) == -1)
                return nullptr;

        return new UnixSocket{ sd };
}
