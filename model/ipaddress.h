#ifndef IPADDRESS_H
#define IPADDRESS_H
#include <QList>

class IPAddress
{
public:
    IPAddress(short pos1, short pos2, short pos3, short pos4) { init(pos1, pos2, pos3, pos4); }
    explicit IPAddress(const QString& ip);

    IPAddress next();

    operator QString() const { return QString::asprintf("%hi%hi%hi%hi", position[0], position[1], position[2], position[3]); }
    friend bool operator==(const IPAddress& l, const IPAddress& r) { return QString(l) == QString(r); }
    friend bool operator!=(const IPAddress& l, const IPAddress& r) { return !(l == r); }
    friend bool operator<(const IPAddress& l, const IPAddress& r) { return QString(l).compare(QString(r)) < 0; }
    friend bool operator>(const IPAddress& l, const IPAddress& r) { return r < l; }
    friend bool operator<=(const IPAddress& l, const IPAddress& r) { return !(l > r); }
private:
    QList<short> position;
    void addAtPosition(short index);
    void init(short pos1, short pos2, short pos3, short pos4);
};

#endif // IPADDRESS_H
