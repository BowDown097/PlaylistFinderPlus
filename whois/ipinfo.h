#ifndef IPINFO_H
#define IPINFO_H
#include <QString>
#include <QVector>

class IPInfo
{
public:
    QString country, mainRange, networkName;
    QVector<QString> otherRanges;

    IPInfo(const QString& _country, const QString& _mainRange, const QString& _networkName, const QVector<QString>& _otherRanges) : country(_country),
        mainRange(_mainRange), networkName(_networkName), otherRanges(_otherRanges) {}
};

#endif // IPINFO_H
