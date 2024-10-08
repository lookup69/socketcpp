#include "fdCourier.h"

using namespace lkup69;

FdCourier::FdCourier(UnixSocket *sockPtr)
{
        m_socketPtr.reset(sockPtr);
}

void FdCourier::operator=(UnixSocket *sockPtr)
{
        m_socketPtr.reset(sockPtr);
}

int FdCourier::Recv(FdPack *fdPackPtr, size_t len)
{
        assert(m_socketPtr);
        assert(fdPackPtr != nullptr);

        char          cmsg[CMSG_SPACE(sizeof(int))];
        int           data = -1;
        struct iovec  io;
        struct msghdr msg = {
                .msg_iov        = &io,
                .msg_iovlen     = 1,
                .msg_control    = cmsg,
                .msg_controllen = sizeof(cmsg)
        };
        int r = -1;

        if (fdPackPtr->extraDataSize > 0) {
                io.iov_base = fdPackPtr->extraDataPtr;
                io.iov_len  = fdPackPtr->extraDataSize;
        } else {
                io.iov_base = cmsg;
                io.iov_len  = sizeof(cmsg);
        }

        if ((r = recvmsg(m_socketPtr->GetSocket(), &msg, sizeof(msg))) < 0)
                return -1;

        struct cmsghdr *c = CMSG_FIRSTHDR(&msg);
        // fdPackPtr->fd     = *(int *)CMSG_DATA(c);  // receive file descriptor
        fdPackPtr->fd = *static_cast<int *>(CMSG_DATA(c));  // receive file descriptor

        return r;
}

int FdCourier::Send(const FdPack *fdPackPtr, size_t len, int flags)
{
        assert(m_socketPtr);
        assert(fdPackPtr != nullptr);

        char          cmsg[CMSG_SPACE(sizeof(int))];
        struct iovec  io;
        struct msghdr msg = {
                .msg_iov        = &io,
                .msg_iovlen     = 1,
                .msg_control    = cmsg,
                .msg_controllen = sizeof(cmsg)
        };

        if (fdPackPtr->extraDataSize > 0) {
                io.iov_base = fdPackPtr->extraDataPtr;
                io.iov_len  = fdPackPtr->extraDataSize;
        } else {
                io.iov_base = cmsg;
                io.iov_len  = sizeof(cmsg);
        }
        struct cmsghdr *c = CMSG_FIRSTHDR(&msg);
        c->cmsg_level     = SOL_SOCKET;
        c->cmsg_type      = SCM_RIGHTS;
        c->cmsg_len       = CMSG_LEN(sizeof(int));
        // *(int *)CMSG_DATA(c) = fdPackPtr->fd;  // set file descriptor
        *static_cast<int *>(CMSG_DATA(c)) = fdPackPtr->fd;  // set file descriptor

        return sendmsg(m_socketPtr->GetSocket(), &msg, 0);
}
