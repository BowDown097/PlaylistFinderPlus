#ifndef IPPORTKEY_H
#define IPPORTKEY_H
#include <QString>

class IPPortKey
{
    QString ip;
    ushort port;
public:
    IPPortKey(const QString& _ip, ushort _port) : ip(_ip), port(_port) {}
    friend bool operator==(const IPPortKey& l, const IPPortKey& r) { return l.ip == r.ip && l.port == r.port; }
    friend bool operator<(const IPPortKey& l, const IPPortKey& r) { return l.ip == r.ip && l.port < r.port; }
};

#endif // IPPORTKEY_H
