/*
 *
 * 2022-07-28
 *
 */

#include "unixSocketClient.h"
#include "fdSocket.h"
using namespace lkup69;

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

struct Data {
        int a;
        int b;
        int c;
};

// g++ -std=c++17 -o fdSocketClientUnitTest fdSocketClientUnitTest.cpp fdSocket.cpp
int main()
{
        UnixSocketClient<FdSocket> usc;

        FdSocket::FdData fdData;

        if (usc.Connect("/tmp/aaa") == -1) {
                printf("Connect fail\n");
                return 0;
        }

        // case 1
        // without extraData
        int fd = open("aaaaa.txt", O_WRONLY | O_CREAT | O_TRUNC, 755);
        if (fd < 0) {
                printf("file open fail\n");
                return -1;
        }
        fdData.fd = fd;

        usc.Send(&fdData, sizeof(fdData));
        printf("S >>> C:%d\n", fdData.fd);


        // cast 2
        // With extraData
        Data             data = {
                            .a = 100,
                            .b = 200,
                            .c = 300
        };

        fd = open("bbbbb.txt", O_WRONLY | O_CREAT | O_TRUNC, 755);
        if (fd < 0) {
                printf("file open fail\n");
                return -1;
        }

        fdData.fd = fd;
        fdData.extraDataPtr  = &data;
        fdData.extraDataSize = sizeof(data);

        usc.Send(&fdData, sizeof(fdData));
        printf("S >>> C:%d\n", fdData.fd);

        return 0;
}

