#include "httpheaders.h"

void HTTPHeaders::add(const QString& ip, ushort port, const QList<QNetworkReply::RawHeaderPair>& headers)
{
    if (!headers.isEmpty())
        cache.insert(IPPortKey(ip, port), headers);
}

bool HTTPHeaders::tryGetHeader(const QString& ip, ushort port, const QString& header, QString& value) const
{
    IPPortKey ipPortKey(ip, port);
    if (cache.contains(ipPortKey))
    {
        const QList<QNetworkReply::RawHeaderPair> pairList = cache[ipPortKey];
        for (auto it = pairList.begin(); it != pairList.end(); ++it)
        {
            if (QString::compare(it->first, header, Qt::CaseInsensitive) == 0)
            {
                value = it->second;
                return true;
            }
        }
    }

    value = QString();
    return false;
}
