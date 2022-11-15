#pragma once

#include "Socket.h"

#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <string>

namespace lkup69
{
class TcpSocket : public Socket
{
public:
        TcpSocket(int socket, int domains);
        ~TcpSocket() override;

        int     GetSocket() override;
        void    Close() override;
        int     Bind(int port) override;
        int     Bind(const std::string &address, int port) override;
        int     Listen(int maxConnection = 1) override;
        Socket *Accept() override;
        int     Connect(const std::string &address, int port) override;
        ssize_t Read(void *buf, size_t len) override;
        ssize_t Write(const void *buf, size_t len) override;
        ssize_t Write(const std::string &msg) override;

        static TcpSocket *CreateSocket(int domain = AF_INET, bool bNonBlocking = false);

private:
        struct sockaddr_in m_inaddr  = { 0 };
        struct sockaddr_in m_cliAddr = { 0 };
        int                m_socket  = -1;
        int                m_domain;
};
}  // namespace lkup69