#pragma once

#include <assert.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include <string>
namespace lkup69
{
class Socket
{
        Socket(const Socket &)            = delete;
        Socket &operator=(const Socket &) = delete;

public:
        Socket(void) = default;
        virtual ~Socket() = default;
        
public:
        virtual void Close()                                    = 0;
        virtual int  GetSocket(void)                            = 0;
        virtual int  Read(void *buf, size_t len)                = 0;
        virtual int  Write(const void *buf, size_t len)         = 0;
        virtual int  Send(const void *buf, size_t len, int flags = 0)
        {
                assert(false);
                return -1;
        }

        virtual int Recv(void *buf, size_t len)
        {
                assert(false);
                return -1;
        }
};

}  // namespace lkup69
