/*
 *
 * 2023-10-19
 *
 */

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <unistd.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/ether.h>

#include <memory>

#include "ifHelper.hpp"

using namespace lkup69;
using namespace std;

struct IfAddrsRAII {
        struct ifaddrs *ifaddr = nullptr;

        IfAddrsRAII() = default;
        ~IfAddrsRAII()
        {
                if (ifaddr)
                        freeifaddrs(ifaddr);
        }

        IfAddrsRAII(const IfAddrsRAII &)     = delete;
        IfAddrsRAII(const IfAddrsRAII &&)    = delete;
        void operator=(const IfAddrsRAII &)  = delete;
        void operator=(const IfAddrsRAII &&) = delete;
};

int IfHelper::GetIfInfo(vector<IfInfo> &ifV)
{
        IfAddrsRAII     ifaddr;
        struct ifaddrs *ifa;
        int             sockfd;

        if (getifaddrs(&ifaddr.ifaddr) == -1)
                return -1;

        sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (sockfd == -1)
                return 1;

        for (ifa = ifaddr.ifaddr; ifa != nullptr; ifa = ifa->ifa_next) {
                if (ifa->ifa_addr != nullptr) {
                        int family = ifa->ifa_addr->sa_family;

                        if ((family == AF_INET) or (family == AF_INET6)) {
                                struct ifreq ifr;
                                char         ip_str[INET6_ADDRSTRLEN]      = { 0 };
                                char         netmask_str[INET6_ADDRSTRLEN] = { 0 };
                                char         mac_str[18]                   = { 0 };

                                if (family == AF_INET) {
                                        struct sockaddr_in *ip      = (struct sockaddr_in *)ifa->ifa_addr;
                                        struct sockaddr_in *netmask = (struct sockaddr_in *)ifa->ifa_netmask;

                                        inet_ntop(AF_INET, &ip->sin_addr, ip_str, sizeof(ip_str));
                                        inet_ntop(AF_INET, &netmask->sin_addr, netmask_str, sizeof(netmask_str));
                                } else {
                                        struct sockaddr_in6 *ip      = (struct sockaddr_in6 *)ifa->ifa_addr;
                                        struct sockaddr_in6 *netmask = (struct sockaddr_in6 *)ifa->ifa_netmask;

                                        inet_ntop(AF_INET6, &ip->sin6_addr, ip_str, sizeof(ip_str));
                                        inet_ntop(AF_INET6, &netmask->sin6_addr, netmask_str, sizeof(netmask_str));
                                }
                                strncpy(ifr.ifr_name, ifa->ifa_name, IFNAMSIZ - 1);
                                if (ioctl(sockfd, SIOCGIFHWADDR, &ifr) == 0) {
                                        struct ether_addr *mac = (struct ether_addr *)ifr.ifr_hwaddr.sa_data;

                                        snprintf(mac_str, sizeof(mac_str), "%02X:%02X:%02X:%02X:%02X:%02X", mac->ether_addr_octet[0], mac->ether_addr_octet[1], mac->ether_addr_octet[2], mac->ether_addr_octet[3], mac->ether_addr_octet[4], mac->ether_addr_octet[5]);
                                }

                                ifV.emplace_back(IfInfo{ ifa->ifa_name, ip_str, netmask_str, (mac_str[0] != 0) ? mac_str : string{} });
                        }
                }
        }

        close(sockfd);

        return 0;
}

// g++ -std=c++17 -DUNIT_TEST -o ifHelper ifHelper.cpp
#ifdef UNIT_TEST
int main()
{
        vector<IfInfo> ifinfoVec;

        if (IfHelper::GetIfInfo(ifinfoVec) == 0) {
                for (auto &it : ifinfoVec) {
                        printf("name:%s\n", it.GetName().c_str());
                        printf("ip  :%s\n", it.GetIp().c_str());
                        printf("mask:%s\n", it.GetMask().c_str());
                        printf("mac :%s\n", it.GetMac().c_str());
                        printf("-------------------------------------\n");
                }
        }
        return 0;
}
#endif