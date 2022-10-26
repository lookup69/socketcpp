#include "../udpSocket.h"

#include <memory>

using namespace lkup69;

// g++ -std=c++17 -o udpSocketClientTest udpSocketClientTest.cpp ../udpSocket.cpp
int main(int argc, char *argv[])
{
        std::unique_ptr<Socket> socketPtr;

        if (argc < 2) {
                printf("unixSocketServerTest address port\n");

                return -1;
        }

        socketPtr.reset(UdpSocket::CreateSocket(AF_INET));
        if (socketPtr == nullptr) {
                printf("Socket Create fail\n");

                return -1;
        }

        if (socketPtr->Connect(argv[1], atoi(argv[2])) == -1) {
                printf("Can not connect to %s\n", argv[1]);

                return -1;
        }

        if (socketPtr->Write("Hello from Client\n") == -1) {
                printf("Write fail\n");

                return -1;
        }

        char buf[1024] = { 0 };

#if 1
        struct sockaddr_in srcAddr = { 0 };
        socklen_t          srclen  = sizeof(srcAddr);
        if (socketPtr->Recvfrom(buf, sizeof(buf) - 1, (struct sockaddr *) &srcAddr, &srclen) == -1) {
                printf("Read fail\n");

                return -1;
        }
        printf("S(%s) ---> C: %s\n", inet_ntoa(srcAddr.sin_addr), buf);
#else
        if (socketPtr->Recv(buf, sizeof(buf), MSG_WAITALL) == -1) {
                printf("Read fail\n");

                return -1;
        }
        printf("S ---> C: %s\n", buf);
#endif

        return 0;
}