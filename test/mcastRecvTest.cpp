#include "../UdpSocket.h"

#include <stdio.h>
#include <error.h>

#include <cstring>
#include <memory>

using namespace lkup69;

// g++ -Wall -std=c++17 -o mcastRecvTest mcastRecvTest.cpp ../UdpSocket.cpp
int main(int argc, char *argv[])
{
        std::unique_ptr<UdpSocket> socketPtr;

        if (argc < 2) {
                printf("upnpSsdpReqTest address port\n");

                return -1;
        }

        socketPtr.reset(UdpSocket::CreateSocket(AF_INET));
        if (socketPtr == nullptr) {
                printf("Socket Create fail\n");

                return -1;
        }

        if(socketPtr->Bind(atoi(argv[2])) == 1) {
                printf("Bind() ... faile\n");

                return -1;
        }
        
        if(socketPtr->McastJoinGroupByIfIndex(argv[1], 0) == -1) {
                printf("McastJoinGroupByIfIndex:%s\n", strerror(errno));

                return -1;
        }

        while (1) {
                char               buf[4096] = { 0 };
#if 1                
                struct sockaddr_in cliaddr   = { 0 };
                socklen_t          clilen = sizeof(cliaddr);

                memset(&cliaddr, 0, sizeof(cliaddr));
                socketPtr->Recvfrom(buf, sizeof(buf) - 1, (struct sockaddr *)&cliaddr, &clilen);
                if((strstr(inet_ntoa(cliaddr.sin_addr), "82.146") == nullptr) && 
                    (strstr(inet_ntoa(cliaddr.sin_addr), "82.40") == nullptr))
                        continue;
                printf("C(%s:%d) ---> S:\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));

#else
                socketPtr->Read(buf, sizeof(buf));
#endif               
                printf("-------------------------------------------\n");
                printf("%s\n", buf);

        }

        return 0;
}