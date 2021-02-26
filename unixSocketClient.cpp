#include "unixSocketClient.h"

using namespace lkup69;

int UnixSocketClient::Connect(const std::string &addr, int port)
{
        int    sd;
        size_t len;

        assert(addr.size() > 0);

        if((sd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
                return -1;

        memset(&m_srvAddr, 0, sizeof(m_srvAddr));
        m_srvAddr.sun_family = AF_UNIX;
        len = (addr.size() > (sizeof(m_srvAddr.sun_path) - 2)) ? (sizeof(m_srvAddr.sun_path) - 2) : addr.size();
        memcpy(m_srvAddr.sun_path, addr.c_str(),  len);
        
        if(connect(sd, (struct sockaddr *) &m_srvAddr, sizeof(struct sockaddr_un)) == -1) {
                return -1;
        }

        m_socket = sd;

        return 0;
}

#ifdef UNIT_TEST
// g++ -DUNIT_TEST -std=c++17 -o unixClient unixSocketClient.cpp
int main()
{
        std::string      msg = "hello this is client";
        UnixSocketClient usc;
        char             buf[256] = {0};

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
