#include "syncSocket.h"
#include "unixSocketClient.h"

using namespace lkup69;

#ifdef UNIT_TEST
g++ -DUNIT_TEST -std=c++17 -o unixClient unixSocketClient.cpp syncSocket.cpp 
int main()
{
        std::string                  msg = "hello this is client";
        UnixSocketClient<SyncSocket> usc;
        char                         buf[256] = {0};

        if(usc.Connect("/tmp/aaa") == -1) {
                printf("Connect fail\n");
                return 0;
        }

        usc.Write(msg.c_str(), msg.size());
        usc.Read(buf, sizeof(buf));
        printf("S >>> C:%s\n", buf);

        return 0;
}
#endif
