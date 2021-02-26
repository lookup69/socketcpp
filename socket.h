#pragma once

#include <assert.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include <string>
namespace lkup69 {
class Socket
{
        Socket(const Socket &) = delete;
        Socket &operator=(const Socket &) = delete;
public:
        Socket(void) = default;

        Socket(int s) : m_socket(s) 
        {
        }

        virtual ~Socket()
        {
                if(m_socket != -1) 
                        close(m_socket);
        }
public:
        virtual int Connect(const std::string &addr, int port = 0)
        {
                return 0;
        }

        virtual int GetSocket(void)
        {
                return m_socket;
        }

        virtual void Close()
        {
                if(m_socket != -1)
                        close(m_socket);

                m_socket = -1;
        }

        virtual int Read(char *buf, size_t len)
        {
                assert(buf != nullptr);
                assert(len > 0);
                assert(m_socket != -1);

                return read(m_socket, buf, len);
        }

        virtual int Write(const char *buf, size_t len)
        {
                assert(buf != nullptr);
                assert(len > 0);
                assert(m_socket != -1);

                return write(m_socket, buf, len);
        }

protected:
        int m_socket = -1;
};

} // namespace