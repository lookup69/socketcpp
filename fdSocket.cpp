/*
 *
 * 2022-07-28
 *
 */

#include "fdSocket.h"

#include <unistd.h>
#include <assert.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>

#include <string>

using namespace lkup69;

FdSocket::FdSocket(int s)
        : m_socket(s)
{
}

FdSocket::~FdSocket()
{
        if (m_socket != -1)
                close(m_socket);
}

int FdSocket::GetSocket(void)
{
        return m_socket;
}

void FdSocket::Close()
{
        if (m_socket != -1)
                close(m_socket);

        m_socket = -1;
}

int FdSocket::Read(void *buf, size_t len)
{
        assert(buf != nullptr);
        assert(len > 0);
        assert(m_socket != -1);

        return Recv(buf, len);
}

int FdSocket::Write(const void *buf, size_t len)
{
        assert(buf != nullptr);
        assert(len > 0);
        assert(m_socket != -1);

        return Send(buf, len);
}

int FdSocket::Recv(void *buf, size_t len)
{
        assert(buf != nullptr);
        assert(len > 0);
        assert(m_socket != -1);

        char          cmsg[CMSG_SPACE(sizeof(int))];
        int           data = -1;
        struct iovec  io;
        struct msghdr msg = {
                .msg_iov        = &io,
                .msg_iovlen     = 1,
                .msg_control    = cmsg,
                .msg_controllen = sizeof(cmsg)
        };
        int        r            = -1;
        FdData *fdDataPtr = (FdData *)buf;

        if (fdDataPtr->extraDataSize > 0) {
                io.iov_base = fdDataPtr->extraDataPtr;
                io.iov_len  = fdDataPtr->extraDataSize;
        } else {
                io.iov_base = cmsg;
                io.iov_len  = sizeof(cmsg);
        }

        if ((r = recvmsg(m_socket, &msg, sizeof(msg))) < 0)
                return -1;

        struct cmsghdr *c = CMSG_FIRSTHDR(&msg);
        fdDataPtr->fd  = *(int *)CMSG_DATA(c);  // receive file descriptor

        return r;
}

int FdSocket::Send(const void *buf, size_t len, int flags)
{
        assert(buf != nullptr);
        assert(len > 0);
        assert(m_socket != -1);

        char          cmsg[CMSG_SPACE(sizeof(int))];
        struct iovec  io;
        struct msghdr msg = {
                .msg_iov        = &io,
                .msg_iovlen     = 1,
                .msg_control    = cmsg,
                .msg_controllen = sizeof(cmsg)
        };
        const FdData *fdDataPtr = (const FdData *)buf;

        if (fdDataPtr->extraDataSize > 0) {
                io.iov_base = fdDataPtr->extraDataPtr;
                io.iov_len  = fdDataPtr->extraDataSize;
        } else {
                io.iov_base = cmsg;
                io.iov_len  = sizeof(cmsg);
        }
        struct cmsghdr *c    = CMSG_FIRSTHDR(&msg);
        c->cmsg_level        = SOL_SOCKET;
        c->cmsg_type         = SCM_RIGHTS;
        c->cmsg_len          = CMSG_LEN(sizeof(int));
        *(int *)CMSG_DATA(c) = fdDataPtr->fd;  // set file descriptor

        return sendmsg(m_socket, &msg, 0);
}



int FdCourier::Recv(void *buf, size_t len)
{
        assert(m_socketPtr);
        assert(buf != nullptr);

        char          cmsg[CMSG_SPACE(sizeof(int))];
        int           data = -1;
        struct iovec  io;
        struct msghdr msg = {
                .msg_iov        = &io,
                .msg_iovlen     = 1,
                .msg_control    = cmsg,
                .msg_controllen = sizeof(cmsg)
        };
        int        r            = -1;
        FdPack *fdPackPtr = (FdPack *)buf;

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
        fdPackPtr->fd  = *(int *)CMSG_DATA(c);  // receive file descriptor

        return r;
}

int FdCourier::Send(const void *buf, size_t len, int flags)
{
        assert(m_socketPtr);
        assert(buf != nullptr);

        char          cmsg[CMSG_SPACE(sizeof(int))];
        struct iovec  io;
        struct msghdr msg = {
                .msg_iov        = &io,
                .msg_iovlen     = 1,
                .msg_control    = cmsg,
                .msg_controllen = sizeof(cmsg)
        };
        const FdPack *fdPackPtr = (const FdPack *)buf;

        if (fdPackPtr->extraDataSize > 0) {
                io.iov_base = fdPackPtr->extraDataPtr;
                io.iov_len  = fdPackPtr->extraDataSize;
        } else {
                io.iov_base = cmsg;
                io.iov_len  = sizeof(cmsg);
        }
        struct cmsghdr *c    = CMSG_FIRSTHDR(&msg);
        c->cmsg_level        = SOL_SOCKET;
        c->cmsg_type         = SCM_RIGHTS;
        c->cmsg_len          = CMSG_LEN(sizeof(int));
        *(int *)CMSG_DATA(c) = fdPackPtr->fd;  // set file descriptor

        return sendmsg(m_socketPtr->GetSocket(), &msg, 0);
}