#pragma once
#include "socket.h"

namespace lkup69 {
class UnixSocketClient : public Socket
{
        UnixSocketClient(const UnixSocketClient &) = delete;
        UnixSocketClient &operator=(const UnixSocketClient&) = delete;
public:
        UnixSocketClient() = default;
        virtual ~UnixSocketClient() = default;
public:
        int Connect(const std::string &addr, int port = 0) override;

private:
        struct sockaddr_un  m_srvAddr;
};
}