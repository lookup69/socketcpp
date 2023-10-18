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

class IfInfo final
{
public:
        IfInfo(const std::string &name,
               const std::string &ip,
               const std::string &mask,
               const std::string &mac)
                : m_if(name, ip, mask, mac)
        {
        }

        ~IfInfo() = default;

        std::string GetName(void)
        {
                return std::get<0>(m_if);
        }

        std::string GetIp(void)
        {
                return std::get<1>(m_if);
        }

        std::string GetMask(void)
        {
                return std::get<2>(m_if);
        }

        std::string GetMac(void)
        {
                return std::get<3>(m_if);
        }

private:
        // name, ip, mask, mac
        std::tuple<std::string, std::string, std::string, std::string> m_if;
};

class IfHelper final
{
public:
        IfHelper()                        = default;
        ~IfHelper()                       = default;
        IfHelper(const IfHelper &)        = delete;
        IfHelper(const IfHelper &&)       = delete;
        void operator=(const IfHelper &)  = delete;
        void operator=(const IfHelper &&) = delete;

        static int GetIfInfo(std::vector<IfInfo> &ifV);
};
}  // namespace lkup69