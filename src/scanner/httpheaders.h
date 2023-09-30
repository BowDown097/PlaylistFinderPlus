#ifndef HTTPHEADERS_H
#define HTTPHEADERS_H
#include "ipportkey.h"
#include <QNetworkReply>

class HTTPHeaders
{
public:
    void add(const QString& ip, ushort port, const QList<QNetworkReply::RawHeaderPair>& headers);
    void clear() { cache.clear(); }
    bool contains(const QString& ip, ushort port) const { return cache.contains(IPPortKey(ip, port)); }
    bool remove(const QString& ip, ushort port) { return cache.remove(IPPortKey(ip, port)) > 0; }
    bool tryGetHeader(const QString& ip, ushort port, const QString& header, QString& value) const;
private:
    QMap<IPPortKey, QList<QNetworkReply::RawHeaderPair>> cache;
};

#endif // HTTPHEADERS_H
