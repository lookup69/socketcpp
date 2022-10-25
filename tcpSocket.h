#pragma once

#include "socket.h"

#include <unistd.h>
#include <sys/types.h> /* See NOTES */
#include <sys/socket.h>
#include <sys/un.h>

#include <string>

namespace lkup69 {
class TcpSocket : public Socket
{
public:

public:
        TcpSocket(int socket);
        ~TcpSocket() override;

        int GetSocket() override;
        void Close() override; 

        int Bind(const std::string &address) override;
        int Connect(const std::string &address) override;
        int Listen(int maxConnection) override;
        UnixSocket *Accept() override;

        int Read(void *buf, size_t len) override;
        int Write(const void *buf, size_t len) override;
        int Write(const std::string &msg) override;

        static TcpSocket *CreateSocket(int domain = AF_INET, bool bNonBlocking = false);

private:
        int                 m_socket = -1;
};
}