#include "../UdpSocket.h"

#include <stdio.h>
#include <error.h>

#include <cstring>
#include <memory>

using namespace lkup69;

static void show_help()
{
        printf("upnpSsdpReqTest -a mcast_address -i interface -p port\n");
        printf("default mcast_address is 239.255.255.250 and port is 1900\n");
        printf("ex:\n");
        printf("\tupnpSsdpReqTest -a 239.255.255.250 -i eth0 -p 1900\n");
}

// g++ -Wall -std=c++17 -o upnpSsdpReqTest upnpSsdpReqTest.cpp ../UdpSocket.cpp
int main(int argc, char *argv[])
{
        std::unique_ptr<UdpSocket> socketPtr;
        std::string                mcastAddress{ "239.255.255.250" };
        std::string                iface;
        int                        port = 1900;
        std::string                msearch{ "M-SEARCH * HTTP/1.1\r\nHOST: 239.255.255.250:1900\r\nMAN: \"ssdp:discover\"\r\nMX: 5\r\nST: urn:schemas-upnp-org:device:MediaRenderer:1\r\nUSER-AGENT: Archer Test Ubuntu\r\n\r\n" };

        for (int i = 0; argv[i] != nullptr; ++i) {
                if (strcmp(argv[i], "--help") == 0) {
                        show_help();
                        exit(0);
                } else if ((strcmp(argv[i], "-a") == 0) && (argv[i + 1] != nullptr)) {
                        mcastAddress = argv[++i];
                } else if ((strcmp(argv[i], "-i") == 0) && (argv[i + 1] != nullptr)) {
                        iface = argv[++i];
                } else if ((strcmp(argv[i], "-p") == 0) && (argv[i + 1] != nullptr)) {
                        port = atoi(argv[++i]);
                }
        }

        socketPtr.reset(UdpSocket::CreateSocket(AF_INET));
        if (socketPtr == nullptr) {
                printf("Socket Create fail\n");

                return -1;
        }

        if (!iface.empty() && (socketPtr->McastJoinGroupByIfName(mcastAddress, iface) == -1)) {
                printf("McastJoinGroupByIfName:%s\n", strerror(errno));
                return -1;
        } else if (socketPtr->McastJoinGroupByIfIndex(mcastAddress, 0) == -1) {
                printf("McastJoinGroupByIfIndex:%s\n", strerror(errno));

                return -1;
        }

        while (1) {
                char               buf[4096] = { 0 };
                struct sockaddr_in cliaddr   = { 0 };
                socklen_t          clilen    = sizeof(cliaddr);

                memset(&cliaddr, 0, sizeof(cliaddr));
                cliaddr.sin_family = AF_INET;
                cliaddr.sin_port   = htons(port);
                inet_aton(mcastAddress.c_str(), &cliaddr.sin_addr);
                if (socketPtr->SendTo(msearch, (struct sockaddr *)&cliaddr, sizeof(cliaddr), 0) == -1) {
                        printf("%s\n", strerror(errno));
                }

                memset(&cliaddr, 0, sizeof(cliaddr));
                socketPtr->Recvfrom(buf, sizeof(buf) - 1, (struct sockaddr *)&cliaddr, &clilen);
                printf("C(%s:%d) ---> S:\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));
                printf("-------------------------------------------\n");
                printf("%s\n", buf);
        }

        return 0;
}