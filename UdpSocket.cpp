#include "UdpSocket.h"

#include <net/if.h>
#include <string.h>
#include <fcntl.h>

using namespace lkup69;

UdpSocket::UdpSocket(int socket, int domain)
        : m_socket(socket),
          m_domain(domain)
{
}

UdpSocket::~UdpSocket()
{
        Close();
}

// static member function
UdpSocket *UdpSocket::CreateSocket(int domain, bool bNonBlocking)
{
        int sd;
        int type = SOCK_DGRAM | SOCK_CLOEXEC;  // when any of the exec-family functions succeed.

        if (bNonBlocking)
                type = type | SOCK_NONBLOCK;

        if ((sd = socket(domain, type, 0)) == -1)
                return nullptr;

        const int on = 1;
        if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) != 0) {
                printf("[%s][%s][%d] setsockopt() ... fail\n", __FILE__, __PRETTY_FUNCTION__, __LINE__);
        }

        return new UdpSocket{ sd, domain };
}

int UdpSocket::GetSocket()
{
        return m_socket;
}

void UdpSocket::Close()
{
        if (m_socket != -1)
                close(m_socket);
        m_socket = -1;
}

int UdpSocket::Bind(int port)
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

int UdpSocket::Bind(const std::string &address, int port)
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

int UdpSocket::Connect(const std::string &address, int port)
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

ssize_t UdpSocket::Read(void *buf, size_t len)
{
        assert(buf);

        return read(m_socket, buf, len);
}

ssize_t UdpSocket::Recv(void *buf, size_t len, int flags)
{
        assert(m_socket != -1);

        return recv(m_socket, buf, len, flags);
}

ssize_t UdpSocket::Recvfrom(void            *buf,
                            size_t           len,
                            struct sockaddr *srcAddr,
                            socklen_t       *addrlen,
                            int              flags)
{
        assert(m_socket != -1);

        return recvfrom(m_socket, buf, len, flags, srcAddr, addrlen);
}

ssize_t UdpSocket::Write(const void *buf, size_t len)
{
        assert(buf);

        return write(m_socket, buf, len);
}

ssize_t UdpSocket::Write(const std::string &msg)
{
        size_t len = msg.size();

        assert(m_socket != -1);

        if (len == 0)
                return -1;

        return write(m_socket, msg.c_str(), len);
}

ssize_t UdpSocket::Send(const std::string &msg, int flags)
{
        size_t len = msg.size();

        assert(m_socket != -1);

        if (len == 0)
                return -1;

        return send(m_socket, msg.c_str(), len, flags);
}

ssize_t UdpSocket::SendTo(const void            *buf,
                          size_t                 len,
                          const struct sockaddr *destAddr,
                          socklen_t              addrlen,
                          int                    flags)
{
        assert(m_socket != -1);

        return sendto(m_socket, buf, len, flags, destAddr, addrlen);
}

ssize_t UdpSocket::SendTo(const std::string     &msg,
                          const struct sockaddr *destAddr,
                          socklen_t              addrlen,
                          int                    flags)
{
        assert(m_socket != -1);

        return sendto(m_socket, msg.c_str(), msg.size(), flags, destAddr, addrlen);
}

int UdpSocket::McastJoinGroupByIfName(const std::string &mcastAddr,
                                      const std::string &ifname)
{
        struct sockaddr_in inaddr = { 0 };
        struct group_req   req    = { 0 };
        int                level  = IPPROTO_IP;

        assert(m_socket != -1);

        if ((req.gr_interface = if_nametoindex(ifname.c_str())) == 0) {
                return -1;
        }

        inaddr.sin_family = m_domain;
        if (m_domain == AF_INET)
                inaddr.sin_addr.s_addr = inet_addr(mcastAddr.c_str());

        memcpy(&req.gr_group, &inaddr, sizeof(inaddr));

        if (m_inaddr.sin_family == AF_INET6)
                level = IPPROTO_IPV6;

        return (setsockopt(m_socket, level, MCAST_JOIN_GROUP, &req, sizeof(req)));
}

int UdpSocket::McastJoinGroupByIfIndex(const std::string &mcastAddr,
                                       size_t             ifindex = 0)
{
        struct sockaddr_in inaddr = { 0 };
        struct group_req   req    = { 0 };
        int                level  = IPPROTO_IP;

        assert(m_socket != -1);

        req.gr_interface  = ifindex;
        inaddr.sin_family = m_domain;

        if (m_domain == AF_INET)
                inaddr.sin_addr.s_addr = inet_addr(mcastAddr.c_str());

        memcpy(&req.gr_group, &inaddr, sizeof(inaddr));

        if (m_inaddr.sin_family == AF_INET6)
                level = IPPROTO_IPV6;

        return (setsockopt(m_socket, level, MCAST_JOIN_GROUP, &req, sizeof(req)));
}

int UdpSocket::McastJoinByIfAddress(const std::string &mcastAddr,
                                    const std::string &ifAddr)
{
        struct ip_mreq mreq  = { 0 };
        int            level = IPPROTO_IP;

        assert(m_socket != -1);

        if (m_inaddr.sin_family == AF_INET6)
                level = IPPROTO_IPV6;

        mreq.imr_interface.s_addr = inet_addr(ifAddr.c_str());
        mreq.imr_multiaddr.s_addr = inet_addr(mcastAddr.c_str());
        return setsockopt(m_socket, level, IP_ADD_MEMBERSHIP, (char *)&mreq, sizeof(mreq));
}

int UdpSocket::McastSetOutGoingIfByAddress(const std::string &ifAddr)
{
        struct in_addr addr = { 0 };

        assert(m_socket != -1);
        addr.s_addr = inet_addr(ifAddr.c_str());

        // IP_MULTICAST_IF:  Sets the interface over which outgoing multicast datagrams are sent.
        return setsockopt(m_socket, IPPROTO_IP, IP_MULTICAST_IF, (char *)&addr, sizeof(addr));
}

int UdpSocket::McastSetLoop(int onoff)
{
        int level = IPPROTO_IP;
        int loop  = IP_MULTICAST_LOOP;  // IP_MULTICAST_LOOP:  Specifies whether or not a copy of
                                        // an outgoing multicast datagram is delivered to the
                                        // sending host as long as it is a member of the multicast group.

        assert(m_socket != -1);

        if (m_domain == AF_INET6) {
                level = IPPROTO_IPV6;
                loop  = IPV6_MULTICAST_LOOP;
        }

        uint8_t flag = onoff;

        return setsockopt(m_socket, level, loop, &flag, sizeof(flag));
}
