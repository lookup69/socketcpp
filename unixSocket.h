#pragma once

#include "socket.h"

#include <unistd.h>
#include <sys/un.h>

#include <string>

namespace lkup69 {
class UnixSocket : public Socket
{
public:

public:
        UnixSocket(int socket);
        ~UnixSocket() override;

        int GetSocket() override;
        void Close() override; 

        int Bind(const std::string &address) override;
        int Connect(const std::string &address) override;
        int Listen(int maxConnection) override;
        UnixSocket *Accept() override;

        ssize_t Read(void *buf, size_t len) override;
        ssize_t Write(const void *buf, size_t len) override;
        ssize_t Write(const std::string &msg) override;

        // type: SOCK_STREAM | SOCK_DGRAM
        static UnixSocket *CreateSocket(int type, bool bNonBlocking = false);

protected:
        int                 m_socket = -1;
};
}