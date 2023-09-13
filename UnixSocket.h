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
        const static int ABSTRACT_SOCKET = 1;        
public:
        explicit UnixSocket(int socket, int type);
        ~UnixSocket() override;

        int  GetSocket() override;
        void Close() override;

        // port would no used. compatiable with socket interface only
        // port = ABSTRACT_SOCKET
        int Bind(const std::string &address, int port = -1) override;

        // abstract socket
        // port = ABSTRACT_SOCKET
        int Connect(const std::string &address, int port = -1) override;
        int Listen(int maxConnection) override;

        UnixSocket *Accept() override;

        ssize_t Read(void *buf, size_t len) override;
        ssize_t Write(const void *buf, size_t len) override;
        ssize_t Write(const std::string &msg) override;

        ssize_t Recv(void *buf, size_t len, int flags = 0) override;
        // port = ABSTRACT_SOCKET
        ssize_t SendTo(const std::string &msg,
                       const std::string &destAddr,
                       int                port  = 0,
                       int                flags = 0) override;

        // type: SOCK_STREAM | SOCK_DGRAM
        static UnixSocket *CreateSocket(int type = SOCK_STREAM, bool bNonBlocking = false);

protected:
        int m_socket = -1;
        int m_type;
};
}  // namespace lkup69
