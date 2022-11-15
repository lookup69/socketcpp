#include "../UnixSocket.h"

#include <memory>

using namespace lkup69;


// g++ -std=c++17 -o unixSocketClientTest unixSocketClientTest.cpp ../UnixSocket.cpp
int main(int argc, char *argv[])
{
        std::unique_ptr<Socket> socketPtr;

        if (argc < 1) {
                printf("unixSocketServerTest address\n");

                return -1;
        }

        socketPtr.reset(UnixSocket::CreateSocket(SOCK_STREAM));
        if (socketPtr == nullptr) {
                printf("Socket Create fail\n");

                return -1;
        }

        if(socketPtr->Connect(argv[1]) == -1) {
                printf("Can not connect to %s\n", argv[1]);

                return -1;
        }

        if(socketPtr->Write("Hello from Client\n") == -1) {
                printf("Write fail\n");

                return -1;
        }

        char buf[1024] = {0};
        if(socketPtr->Read(buf, sizeof(buf)) == -1) {
                printf("Read fail\n");

                return -1;
        }

        printf("S ---> C: %s\n", buf);

        return 0;
}