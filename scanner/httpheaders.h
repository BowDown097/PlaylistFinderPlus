#ifndef HTTPHEADERS_H
#define HTTPHEADERS_H
#include "model/ipportkey.h"
#include <QMap>
#include <QSet>

class HTTPHeaders
{
public:
    bool contains(const QString& ip, ushort port)
    {
        return cache.contains(IPPortKey(ip, port));
    }
private:
    QMap<IPPortKey, QMap<QString, QSet<QString>>> cache;
};

#endif // HTTPHEADERS_H
