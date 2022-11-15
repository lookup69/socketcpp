#include "../UdpSocket.h"

#include <memory>

using namespace lkup69;

// g++ -std=c++17 -o udpSocketServerTest udpSocketServerTest.cpp ../UdpSocket.cpp
int main(int argc, char *argv[])
{
        std::unique_ptr<Socket> socketPtr;

        if (argc < 1) {
                printf("unixSocketServerTest port\n");

                return -1;
        }

        socketPtr.reset(UdpSocket::CreateSocket(AF_INET));
        if (socketPtr == nullptr) {
                printf("Socket Create fail\n");

                return -1;
        }

        if (socketPtr->Bind(atoi(argv[1])) == -1) {
                printf("Socket bind fail\n");

                return -1;
        }

        while (1) {
                char               buf[1024] = { 0 };
                struct sockaddr_in cliaddr   = { 0 };
                socklen_t          clilen    = sizeof(cliaddr);

                socketPtr->Recvfrom(buf, sizeof(buf) - 1, (struct sockaddr *) &cliaddr, &clilen);

                printf("C(%s) ---> S: %s\n", inet_ntoa(cliaddr.sin_addr), buf);
                socketPtr->SendTo(std::string{ "Hello by Server" }, (struct sockaddr *) &cliaddr, clilen);
        }

        return 0;
}