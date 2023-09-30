#ifndef HTTPHEADERS_H
#define HTTPHEADERS_H
#include "ipportkey.h"
#include <QNetworkReply>

class HttpHeaders
{
public:
    void add(const QString& ip, ushort port, const QList<QNetworkReply::RawHeaderPair>& headers);
    void clear() { cache.clear(); }
    bool contains(const QString& ip, ushort port) const { return cache.contains(IpPortKey(ip, port)); }
    bool remove(const QString& ip, ushort port) { return cache.remove(IpPortKey(ip, port)) > 0; }
    bool tryGetHeader(const QString& ip, ushort port, const QString& header, QString& value) const;
private:
    QMap<IpPortKey, QList<QNetworkReply::RawHeaderPair>> cache;
};

#endif // HTTPHEADERS_H
