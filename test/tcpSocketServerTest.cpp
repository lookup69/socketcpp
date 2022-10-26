#include "../tcpSocket.h"

#include <memory>

using namespace lkup69;

// g++ -std=c++17 -o tcpSocketServerTest tcpSocketServerTest.cpp ../tcpSocket.cpp
int main(int argc, char *argv[])
{
        std::unique_ptr<Socket> socketPtr;

        if (argc < 1) {
                printf("unixSocketServerTest port\n");

                return -1;
        }

        socketPtr.reset(TcpSocket::CreateSocket(AF_INET));
        if (socketPtr == nullptr) {
                printf("Socket Create fail\n");

                return -1;
        }

        if (socketPtr->Bind(atoi(argv[1])) == -1) {
                printf("Socket bind fail\n");

                return -1;
        }

        if (socketPtr->Listen() < 0) {
                printf("Socket lisent fail\n");

                return -1;
        }

        while (1) {
                char                    buf[1024] = { 0 };
                std::unique_ptr<Socket> cliSocketPtr;

                cliSocketPtr.reset(socketPtr->Accept());

                if (cliSocketPtr == nullptr) {
                        printf("Accpet fail\n");
                        continue;
                }

                cliSocketPtr->Read(buf, sizeof(buf));
                printf("[C --> S]:%s\n", buf);

                cliSocketPtr->Write(std::string("Say hello from Server"));
        }

        return 0;
}