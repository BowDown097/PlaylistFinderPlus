#ifndef WHOISCLIENT_H
#define WHOISCLIENT_H
#include "ipinfo.h"
#include "whoisresult.h"
#include <QNetworkAccessManager>

class WhoisClient : public QObject
{
    Q_OBJECT
public:
    explicit WhoisClient(QObject* parent = nullptr)
        : QObject(parent), m_networkManager(new QNetworkAccessManager(this)) {}

    IpInfo getIpInfo(const QString& ip, ushort port);
private:
    QNetworkAccessManager* m_networkManager;

    QString findStringAndGetValue(const QStringList& lines, const QString& searchString);
    QByteArray getSiteText(const QString& uri);
    WhoisResult getWhoisData(const QString& ip, ushort port);
};

#endif // WHOISCLIENT_H
