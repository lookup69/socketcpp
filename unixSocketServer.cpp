#include "unixSocketServer.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <assert.h>
#include <string.h>

using namespace lkup69;

UnixSocketServer::~UnixSocketServer()
{
        if(m_socket != -1) 
                close(m_socket);
        remove(m_srvAddr.sun_path);                
}

int UnixSocketServer::Init()
{
        size_t len;

        assert(m_addr.size() > 0);

        memset(&m_srvAddr, 0, sizeof(m_srvAddr));
        m_srvAddr.sun_family = AF_UNIX;

        //Abstract Socket Namespace
        len = (m_addr.size() > (sizeof(m_srvAddr.sun_path) - 2)) ? (sizeof(m_srvAddr.sun_path) - 2) : m_addr.size();
        memcpy(m_srvAddr.sun_path, m_addr.c_str(),  len);

        m_socket = socket(AF_UNIX, SOCK_STREAM, 0);
        if(m_socket < 0) {
                return -1;
        }

        unlink(m_srvAddr.sun_path);
        if(bind(m_socket, (struct sockaddr *) &m_srvAddr, sizeof(struct sockaddr_un)) == -1) {
                close(m_socket);
                m_socket = -1;
                return -1;
        }

        return listen(m_socket, m_maxConnection);
}

std::unique_ptr<Socket> UnixSocketServer::Accept(void)
{
        int sd;

        if((sd = accept(m_socket, NULL, 0)) == -1)
                return nullptr;

        return std::move(std::unique_ptr<Socket>(new Socket(sd)));
}

#ifdef UNIT_TEST
// g++ -DUNIT_TEST -std=c++17 -o unixServer unixSocketServer.cpp

int main()
{
        UnixSocketServer sr = SocketServer::Builder<UnixSocketServer>().SetAddress("/tmp/aaa").Build();

        if(sr.Init() !=0) {
                printf("UnixSocketServer init() fail\n");
                exit(0);
        }

        while(1) {
                std::string str = "hello this is server";

                std::unique_ptr<Socket> s = sr.Accept();
                
                char buf[256] = {0};
                s->Read(buf, sizeof(buf));
                printf("C >>> S:%s\n", buf);

                s->Write(str.c_str(), str.size());
        }

        return 0;
}
#endif