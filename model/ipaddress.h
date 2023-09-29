#ifndef IPADDRESS_H
#define IPADDRESS_H
#include <QList>

class IPAddress
{
public:
    IPAddress(ushort pos1, ushort pos2, ushort pos3, ushort pos4) { init(pos1, pos2, pos3, pos4); }
    explicit IPAddress(const QString& ip);

    IPAddress next();

    operator QString() const { return QString::asprintf("%hi%hi%hi%hi", position[0], position[1], position[2], position[3]); }
    friend bool operator==(const IPAddress& l, const IPAddress& r) { return QString(l) == QString(r); }
    friend bool operator!=(const IPAddress& l, const IPAddress& r) { return !(l == r); }
    friend bool operator<(const IPAddress& l, const IPAddress& r) { return QString(l).compare(QString(r)) < 0; }
    friend bool operator>(const IPAddress& l, const IPAddress& r) { return r < l; }
    friend bool operator<=(const IPAddress& l, const IPAddress& r) { return !(l > r); }
private:
    QList<ushort> position;
    void addAtPosition(ushort index);
    void init(ushort pos1, ushort pos2, ushort pos3, ushort pos4);
};

#endif // IPADDRESS_H
