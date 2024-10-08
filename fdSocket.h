/*
 *
 * 2022-07-28
 *
 */

#pragma once

#include <string>

#include <memory>

#include "socket.h"
#include "unixSocket.h"

namespace lkup69
{
class FdSocket : public Socket
{
        FdSocket(const Socket &)              = delete;
        FdSocket &operator=(const FdSocket &) = delete;

public:
        struct FdData {
                void  *extraDataPtr  = nullptr;
                size_t extraDataSize = 0;
                int    fd            = -1;
        };

public:
        FdSocket(void) = default;
        FdSocket(int s);
        ~FdSocket() override;

public:
        static FdSocket *CreateSocket(int fd);

public:
        int  GetSocket(void) override;
        void Close() override;
        int  Read(void *buf, size_t len) override;
        int  Write(const void *buf, size_t len) override;
        int  Recv(void *buf, size_t len) override;
        int  Send(const void *buf, size_t len, int flags = 0) override;

protected:
        int m_socket = -1;
};

class FdCourier
{
public:
        struct FdPack {
                void  *extraDataPtr  = nullptr;
                size_t extraDataSize = 0;
                int    fd            = -1;
        };

        int Recv(void *buf, size_t len);
        int Send(const void *buf, size_t len, int flags = 0);

private:
        std::unique_ptr<UnixSocket> m_socketPtr;
};
}  // namespace lkup69
