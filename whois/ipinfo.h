#ifndef IPINFO_H
#define IPINFO_H
#include <QStringList>

class IPInfo
{
public:
    QString country;
    QString mainRange;
    QString networkName;
    QStringList otherRanges;

    IPInfo(const QString& country, const QString& mainRange, const QString& networkName, const QStringList& otherRanges)
        : country(country), mainRange(mainRange), networkName(networkName), otherRanges(otherRanges) {}
};

#endif // IPINFO_H
