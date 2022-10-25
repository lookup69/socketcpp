#pragma once

#include <memory>

#include "unixSocket.h"

namespace lkup69
{
class FdCourier
{
public:
        struct FdPack {
                void  *extraDataPtr  = nullptr;
                size_t extraDataSize = 0;
                int    fd            = -1;
        };

public:
        FdCourier() = default;
        FdCourier(UnixSocket *sockPtr);
        FdCourier(const FdCourier &) = delete;
        void operator=(UnixSocket *sockPtr);

        int Recv(FdPack *buf, size_t len);
        int Send(const FdPack *buf, size_t len, int flags = 0);

private:
        std::unique_ptr<UnixSocket> m_socketPtr;
};
}  // namespace lkup69