#include "../unixSocket.h"

#include <memory>

using namespace lkup69;

// g++ -std=c++17 -o unixSocketServerTest unixSocketServerTest.cpp ../unixSocket.cpp
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

        unlink(argv[1]);
        if (socketPtr->Bind(argv[1]) == -1) {
                printf("Socket bind fail\n");

                return -1;
        }

        if(socketPtr->Listen(1) == -1) {
                printf("Socket Listen  fail\n");

                return -1;
        }

        while (1) {
                std::unique_ptr<Socket> clientPtr;
                char                    buf[1024] = { 0 };

                clientPtr.reset(socketPtr->Accept());
                if (clientPtr == nullptr)
                        continue;
                clientPtr->Read(buf, sizeof(buf) - 1);
                printf("C ---> S: %s\n", buf);
                clientPtr->Write(std::string{ "Hello by Server" });
        }

        return 0;
}