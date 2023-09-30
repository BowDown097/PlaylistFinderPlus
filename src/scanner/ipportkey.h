#ifndef IPPORTKEY_H
#define IPPORTKEY_H
#include <QString>

class IPPortKey
{
public:
    IPPortKey(const QString& ip, ushort port) : m_ip(ip), m_port(port) {}
    friend bool operator==(const IPPortKey& l, const IPPortKey& r) { return l.m_ip == r.m_ip && l.m_port == r.m_port; }

    QString ip() const { return m_ip; }
    ushort port() const { return m_port; }
private:
    QString m_ip;
    ushort m_port;
};

#endif // IPPORTKEY_H
