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

#include "IfaceHelper.hpp"

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

string IfaceHelper::GetIfaceMacAddr(const string &name, int s /*reuse socket*/)
{
        int  sockfd      = s;
        char mac_str[18] = { 0 };

        auto getMac__ = [&sockfd, &mac_str, &name]() {
                struct ifreq ifr;

                strncpy(ifr.ifr_name, name.c_str(), IFNAMSIZ - 1);
                if (ioctl(sockfd, SIOCGIFHWADDR, &ifr) == 0) {
                        struct ether_addr *mac = (struct ether_addr *)ifr.ifr_hwaddr.sa_data;

                        snprintf(mac_str, sizeof(mac_str),
                                 "%02X:%02X:%02X:%02X:%02X:%02X",
                                 mac->ether_addr_octet[0],
                                 mac->ether_addr_octet[1],
                                 mac->ether_addr_octet[2],
                                 mac->ether_addr_octet[3],
                                 mac->ether_addr_octet[4],
                                 mac->ether_addr_octet[5]);
                }
        };

        if (sockfd < 0) {
                if (sockfd = socket(AF_INET, SOCK_DGRAM, 0); sockfd != -1) {
                        getMac__();
                        close(sockfd);
                }
        } else {
                getMac__();
        }

        return (mac_str[0] != 0) ? string{ mac_str } : string{};
}

int IfaceHelper::GetAllIfaceInfo(vector<IfaceInfo> &ifaceV)
{
        IfAddrsRAII     ifaddrRaii;
        struct ifaddrs *ifa;
        int             sockfd;

        if (getifaddrs(&ifaddrRaii.ifaddr) == -1)
                return -1;

        if (sockfd = socket(AF_INET, SOCK_DGRAM, 0); sockfd == -1)
                return -1;

        for (ifa = ifaddrRaii.ifaddr; ifa != nullptr; ifa = ifa->ifa_next) {
                if (ifa->ifa_addr != nullptr) {
                        int family = ifa->ifa_addr->sa_family;

                        if ((family == AF_INET) or (family == AF_INET6)) {
                                char ip_str[INET6_ADDRSTRLEN]      = { 0 };
                                char netmask_str[INET6_ADDRSTRLEN] = { 0 };

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

                                ifaceV.emplace_back(IfaceInfo{ ifa->ifa_name, ip_str, netmask_str, std::move(GetIfaceMacAddr(ifa->ifa_name, sockfd)) });
                        }
                }
        }

        close(sockfd);

        return 0;
}

// g++ -std=c++17 -DUNIT_TEST -o IfaceHelper IfaceHelper.cpp
#ifdef UNIT_TEST
int main()
{
        vector<IfaceInfo> IfaceInfoVec;

        if (IfaceHelper::GetAllIfaceInfo(IfaceInfoVec) == 0) {
                for (auto &it : IfaceInfoVec) {
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