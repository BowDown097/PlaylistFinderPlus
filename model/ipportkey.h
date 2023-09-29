#ifndef IPPORTKEY_H
#define IPPORTKEY_H
#include <QString>

class IPPortKey
{
public:
    IPPortKey(const QString& ip, ushort port) : ip(ip), port(port) {}
    friend bool operator==(const IPPortKey& l, const IPPortKey& r) { return l.ip == r.ip && l.port == r.port; }
    friend bool operator<(const IPPortKey& l, const IPPortKey& r) { return l.ip == r.ip && l.port < r.port; }
private:
    QString ip;
    ushort port;
};

#endif // IPPORTKEY_H
