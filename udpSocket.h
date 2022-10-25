#pragma once

#include "socket.h"

#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <string>

namespace lkup69
{
class UdpSocket : public Socket
{
public:
        UdpSocket(int socket, int domains);
        ~UdpSocket() override;

        int  GetSocket() override;
        void Close() override;

        int Bind(int port) override;
        int Bind(const std::string &address, int port) override;
        int Connect(const std::string &address, int port) override;

        ssize_t Read(void *buf, size_t len) override;
        ssize_t Recv(void *buf, size_t len, int flags = 0) override;
        ssize_t Recvfrom(void            *buf,
                         size_t           len,
                         struct sockaddr *srcAddr = nullptr,
                         socklen_t       *addrlen = nullptr,
                         int              flags   = 0) override;
        ssize_t Write(const void *buf, size_t len) override;
        ssize_t Write(const std::string &msg) override;
        ssize_t Send(const std::string &msg, int flags = 0) override;
        ssize_t SendTo(const void            *buf,
                       size_t                 len,
                       const struct sockaddr *destAddr,
                       socklen_t              addrlen,
                       int                    flags = 0) override;

        ssize_t SendTo(const std::string     &msg,
                       const struct sockaddr *destAddr,
                       socklen_t              addrlen,
                       int                    flags = 0) override;

        int McastJoinGroupByIfName(const std::string &mcastAddr,
                                   const std::string &ifname);
        int McastJoinGroupByIfIndex(const std::string &mcastAddr,
                                    size_t             ifindex);
        int McastJoinByIfAddress(const std::string &mcastAddr,
                               const std::string &ifAddr = std::string{});
        int McastSetOutGoingIfByAddress(const std::string &ifAddr);
        int McastSetLoop(int onoff);

        static UdpSocket *CreateSocket(int domain = AF_INET, bool bNonBlocking = false);

private:
        struct sockaddr_in m_inaddr = { 0 };
        int                m_socket = -1;
        int                m_domain;
};
}  // namespace lkup69