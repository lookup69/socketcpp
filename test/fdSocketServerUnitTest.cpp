/*
 *
 * 2022-07-28
 *
 */


#include "unixSocketServer.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <assert.h>
#include <string.h>

#include "fdSocket.h"

using namespace lkup69;

// g++ -std=c++17 -o fdSocketServerUnitTest fdSocketServerUnitTest.cpp  fdSocket.cpp

struct Data {
        int a;
        int b;
        int c;
};

int main()
{
        std::unique_ptr<UnixSocketServer<FdSocket>> srv;

        srv.reset(SocketServer::Builder<UnixSocketServer<FdSocket>>().SetAddress("/tmp/aaa").BuildPtr());

        if (srv->Init() != 0) {
                printf("init() fail\n");
                exit(0);
        }

        while (1) {
                std::unique_ptr<Socket> s = srv->Accept();
                FdSocket::FdData        fdData;

                // Case 1
                // Without extraData
                s->Read(&fdData, sizeof(fdData));
                printf("C >>> S:%d\n", fdData.fd);

                std::string txt = "xxxxaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n";
                if (fdData.fd > -1) {
                        write(fdData.fd, txt.c_str(), txt.size());
                        close(fdData.fd);
                }

                // Case 2
                // With extraData
                Data data;

                fdData.extraDataPtr  = &data;
                fdData.extraDataSize = sizeof(data);
                s->Read(&fdData, sizeof(fdData));
                printf("C >>> S:%d\n", fdData.fd);

                printf("data.a:%d\n", data.a);
                printf("data.b:%d\n", data.b);
                printf("data.c:%d\n", data.c);

                if (fdData.fd > -1) {
                        write(fdData.fd, txt.c_str(), txt.size());
                        close(fdData.fd);
                }
        }

        return 0;
}
