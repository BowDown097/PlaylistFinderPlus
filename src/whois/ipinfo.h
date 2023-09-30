#ifndef IPINFO_H
#define IPINFO_H
#include <QStringList>

class IpInfo
{
public:
    IpInfo(const QString& country, const QString& networkName, const QString& mainRange, const QStringList& otherRanges)
        : m_country(country), m_mainRange(mainRange), m_networkName(networkName), m_otherRanges(otherRanges) {}

    QString country() const { return m_country; }
    QString mainRange() const { return m_mainRange; }
    QString networkName() const { return m_networkName; }
    QStringList otherRanges() const { return m_otherRanges; }
private:
    QString m_country;
    QString m_mainRange;
    QString m_networkName;
    QStringList m_otherRanges;
};

#endif // IPINFO_H
