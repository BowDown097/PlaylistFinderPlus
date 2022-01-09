#ifndef IPADDRESS_H
#define IPADDRESS_H
#include <QList>
#include <QString>
#include <QVector>

class IPAddress
{
    QVector<short> position;

    void init(short pos1, short pos2, short pos3, short pos4)
    {
        position.append({pos1, pos2, pos3, pos4});
    }

    void addAtPosition(short index)
    {
        if (index == 0) return;

        if (position[index] != 255)
        {
            position[index]++;
            return;
        }

        position[index] = 0;
        addAtPosition(--index);
    }
public:
    IPAddress(short pos1, short pos2, short pos3, short pos4)
    {
        init(pos1, pos2, pos3, pos4);
    }

    IPAddress(const QString& ip)
    {
        QVector<short> vec;
        foreach (QStringRef s, ip.splitRef('.'))
            vec.append(s.toShort());
        init(vec[0], vec[1], vec[2], vec[3]);
    }

    IPAddress next()
    {
        IPAddress address(position[0], position[1], position[2], position[3]);
        address.addAtPosition(3);
        return address;
    }

    operator QString() const { return QString::asprintf("%hi%hi%hi%hi", position[0], position[1], position[2], position[3]); }
    friend bool operator==(const IPAddress& l, const IPAddress& r) { return QString(l) == QString(r); }
    friend bool operator!=(const IPAddress& l, const IPAddress& r) { return !(l == r); }
    friend bool operator<(const IPAddress& l, const IPAddress& r) { return QString(l).compare(QString(r)) < 0; }
    friend bool operator>(const IPAddress& l, const IPAddress& r) { return r < l; }
    friend bool operator<=(const IPAddress& l, const IPAddress& r) { return !(l > r); }
};

#endif // IPADDRESS_H
