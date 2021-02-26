#pragma once

#include "socket.h"
#include "socketServer.h"

namespace lkup69
{
class UnixSocketServer : public SocketServer
{
        UnixSocketServer(const UnixSocketServer &) = delete;
        UnixSocketServer &operator=(const UnixSocketServer&) = delete;
public:
        UnixSocketServer(const std::string &addr, int port, int maxConnection) :
                m_addr(addr), m_maxConnection(maxConnection)
        {
        }

        ~UnixSocketServer() override;

public:
        int Init(void) override;
        std::unique_ptr<Socket> Accept(void) override;

private:
        struct sockaddr_un  m_srvAddr;
        std::string         m_addr;
        int                 m_socket = -1;
        int                 m_maxConnection = -1;
};
}

