#ifndef WHOIS_H
#define WHOIS_H
#include "ipinfo.h"
#include <QNetworkAccessManager>

class Whois
{
public:
    IPInfo getIpInfo(QNetworkAccessManager* manager, const QString& ip, ushort port);
};

#endif // WHOIS_H
