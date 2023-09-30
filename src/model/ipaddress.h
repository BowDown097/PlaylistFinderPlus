#ifndef IPADDRESS_H
#define IPADDRESS_H
#include <QList>

class IpAddress
{
public:
    IpAddress(ushort pos1, ushort pos2, ushort pos3, ushort pos4) { init(pos1, pos2, pos3, pos4); }
    explicit IpAddress(const QString& ip);

    IpAddress next();

    operator QString() const;
    friend bool operator==(const IpAddress& l, const IpAddress& r) { return l.positionAsString == r.positionAsString; }
    friend bool operator!=(const IpAddress& l, const IpAddress& r) { return !(l == r); }
    friend bool operator<(const IpAddress& l, const IpAddress& r) { return l.positionAsString.compare(r.positionAsString) < 0; }
    friend bool operator>(const IpAddress& l, const IpAddress& r) { return r < l; }
    friend bool operator<=(const IpAddress& l, const IpAddress& r) { return !(l > r); }
private:
    QList<ushort> position;
    QString positionAsString;

    void addAtPosition(ushort index);
    QString asString() const;
    void init(ushort pos1, ushort pos2, ushort pos3, ushort pos4);
};

#endif // IPADDRESS_H
