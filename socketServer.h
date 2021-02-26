#pragma once

#include <string>
#include <memory>

#include "socket.h"

namespace lkup69 {

class SocketServer
{
public:        
        template <typename T>
        class Builder
        {
        public:
                Builder() = default;
                ~Builder() = default;

                Builder &SetAddress(const std::string &addr)
                {
                        m_addr = addr;

                        return *this;
                }

                Builder &SetPort(const std::string &addr)
                {
                        m_addr = addr;

                        return *this;
                }
                
                Builder &SetMaxConnection(int conns)
                {
                        m_maxConns = conns;

                        return *this;
                }

                T Build()
                {
                        return T(m_addr, m_port, m_maxConns);
                }

        private:
                std::string m_addr;
                int         m_port = 0;
                int         m_maxConns = 1;
        };
        
// Constructor
        SocketServer(const SocketServer &) = delete;
        SocketServer &operator=(const SocketServer&) = delete;
public:
        SocketServer() = default;
        SocketServer(const std::string &addr, int port, int maxConnection)
        {
        }
        
        virtual ~SocketServer() = default;

// Interface
        virtual int Init() = 0;
        virtual std::unique_ptr<Socket> Accept(void) = 0;
};

} //namespace
