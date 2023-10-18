/*
 *
 * 2023-10-19
 *
 */

#include <string>
#include <tuple>
#include <vector>

namespace lkup69
{

class IfaceInfo final
{
public:
        IfaceInfo(const std::string &name,
                  const std::string &ip,
                  const std::string &mask,
                  const std::string &mac)
                : m_iface(name, ip, mask, mac)
        {
        }

        ~IfaceInfo() = default;

        std::string GetName(void)
        {
                return std::get<0>(m_iface);
        }

        std::string GetIp(void)
        {
                return std::get<1>(m_iface);
        }

        std::string GetMask(void)
        {
                return std::get<2>(m_iface);
        }

        std::string GetMac(void)
        {
                return std::get<3>(m_iface);
        }

private:
        // name, ip, mask, mac
        std::tuple<std::string, std::string, std::string, std::string> m_iface;
};

class IfaceHelper final
{
public:
        IfaceHelper()                        = default;
        ~IfaceHelper()                       = default;
        IfaceHelper(const IfaceHelper &)     = delete;
        IfaceHelper(const IfaceHelper &&)    = delete;
        void operator=(const IfaceHelper &)  = delete;
        void operator=(const IfaceHelper &&) = delete;

        static int GetIfaceInfo(std::vector<IfaceInfo> &ifaceV);
};
}  // namespace lkup69