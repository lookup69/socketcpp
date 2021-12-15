#include "syncSocket.h"
#include "unixSocketServer.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <assert.h>
#include <string.h>

using namespace lkup69;

#ifdef UNIT_TEST
// g++ -DUNIT_TEST -std=c++17 -o unixServer unixSocketServer.cpp syncSocket.cpp 

int main()
{
        UnixSocketServer sr = SocketServer::Builder<UnixSocketServer<SyncSocket>>().SetAddress("/tmp/aaa").Build();

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