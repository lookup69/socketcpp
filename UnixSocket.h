#pragma once

#include "Socket.h"

#include <unistd.h>
#include <sys/un.h>

#include <string>

namespace lkup69
{
class UnixSocket : public Socket
{
public:
        const static int NORMAL_SOCKET   = 0;
        const static int ABSTRACT_SOCKET = 1;

public:
        explicit UnixSocket(int socket, int type, bool bConnected = true);
        ~UnixSocket() override;

        int  GetSocket() override;
        void Close() override;

        // port would no used. compatiable with socket interface only
        int Bind(const std::string &address, int port = NORMAL_SOCKET) override;
        int Connect(const std::string &address) override;
        int Connect(const std::string &address, int port = NORMAL_SOCKET) override;
        int Listen(int maxConnection) override;

        UnixSocket *Accept() override;

        ssize_t Read(void *buf, size_t len) override;
        ssize_t Write(const void *buf, size_t len) override;
        ssize_t Write(const std::string &msg) override;

        ssize_t Recv(void *buf, size_t len, int flags = 0) override;
        ssize_t SendTo(const std::string &msg,
                       const std::string &destAddr,
                       int                port  = NORMAL_SOCKET,
                       int                flags = 0) override;

        // type: SOCK_STREAM | SOCK_DGRAM
        static UnixSocket *CreateSocket(int type = SOCK_STREAM, bool bNonBlocking = false);

protected:
        int  m_socket = -1;
        int  m_type;
        bool m_bConnected = false;
};
}  // namespace lkup69
