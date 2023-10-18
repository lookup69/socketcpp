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
        const static size_t kIFaceInfoField_NAME = 0;
        const static size_t kIFaceInfoField_IP   = 1;
        const static size_t kIFaceInfoField_MASK = 2;
        const static size_t kIFaceInfoField_MAC  = 3;

public:
        IfaceInfo(const std::string &name,
                  const std::string &ip,
                  const std::string &mask,
                  const std::string &mac)
        {
                std::get<kIFaceInfoField_NAME>(m_iface) = name;
                std::get<kIFaceInfoField_IP>(m_iface)   = ip;
                std::get<kIFaceInfoField_MASK>(m_iface) = mask;
                std::get<kIFaceInfoField_MAC>(m_iface)  = mac;
        }

        ~IfaceInfo() = default;

        std::string GetName(void)
        {
                return std::get<kIFaceInfoField_NAME>(m_iface);
        }

        std::string GetIp(void)
        {
                return std::get<kIFaceInfoField_IP>(m_iface);
        }

        std::string GetMask(void)
        {
                return std::get<kIFaceInfoField_MASK>(m_iface);
        }

        std::string GetMac(void)
        {
                return std::get<kIFaceInfoField_MAC>(m_iface);
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