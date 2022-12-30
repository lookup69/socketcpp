#include "../UdpSocket.h"

#include <stdio.h>
#include <error.h>

#include <vector>
#include <cstring>
#include <memory>

using namespace lkup69;

static void show_help()
{
        printf("upnpRecvTest [-f filter_ip]\n");
        printf("\t-f only show the filter_ip package");
        printf("ex:\n");
        printf("\tupnpRecvTest \n");
        printf("\tupnpRecvTest -f 1.1.1.1 -f 1.1.1.2 \n");
}

// g++ -Wall -std=c++17 -o upnpRecvTest upnpRecvTest.cpp ../UdpSocket.cpp
int main(int argc, char *argv[])
{
        std::unique_ptr<UdpSocket> socketPtr;
        std::vector<std::string>   ipFilterVec;
        std::string                mcastAddress{ "239.255.255.250" };
        int                        port = 1900;

        for (int i = 0; argv[i] != nullptr; ++i) {
                if (strcmp(argv[i], "--help") == 0) {
                        show_help();
                        exit(0);
                } else if ((strcmp(argv[i], "-f") == 0) && (argv[i + 1] != nullptr)) {
                        ipFilterVec.push_back(argv[++i]);
                }
        }

        socketPtr.reset(UdpSocket::CreateSocket(AF_INET));
        if (socketPtr == nullptr) {
                printf("Socket Create fail\n");

                return -1;
        }

        if (socketPtr->Bind(mcastAddress, port) == 1) {
                printf("Bind() ... faile\n");

                return -1;
        }

        while (1) {
                char               buf[4096] = { 0 };
                struct sockaddr_in cliaddr   = { 0 };
                socklen_t          clilen    = sizeof(cliaddr);

                memset(&cliaddr, 0, sizeof(cliaddr));
                socketPtr->Recvfrom(buf, sizeof(buf) - 1, (struct sockaddr *)&cliaddr, &clilen);
                if (ipFilterVec.size() > 0) {
                        for (auto &ip : ipFilterVec) {
                                if (ip.compare(inet_ntoa(cliaddr.sin_addr)) == 0) {
                                        printf("C(%s:%d) ---> S:\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));
                                        printf("-------------------------------------------\n");
                                        printf("%s\n", buf);
                                }
                        }
                } else {
                        printf("C(%s:%d) ---> S:\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));
                        printf("-------------------------------------------\n");
                        printf("%s\n", buf);
                }
        }

        return 0;
}