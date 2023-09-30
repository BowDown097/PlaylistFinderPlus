#ifndef DDNS_H
#define DDNS_H
#include <QString>

class Ddns
{
public:
    Ddns() = default;
    Ddns(const QString& name, const QString& url) : m_name(name), m_url(url) {}

    QString name() const { return m_name; }
    QString url() const { return m_url; }
private:
    QString m_name;
    QString m_url;
};

#endif // DDNS_H
