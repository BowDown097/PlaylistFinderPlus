#ifndef IPADDRESS_H
#define IPADDRESS_H
#include <QList>
#include <QString>
#include <QVector>

class IPAddress
{
    QVector<short> position;
    QStringView positionAsString;

    void init(const short& pos1, const short& pos2, const short& pos3, const short& pos4)
    {
        position.append({ pos1, pos2, pos3, pos4});
        positionAsString = asString();
    }

    QStringView asString()
    {
        return QStringLiteral("%1%2%3%4").arg(QString::number(position[0]), QString::number(position[1]), QString::number(position[2]), QString::number(position[3]));
    }

    void addAtPosition(short index)
    {
        if (index == 0) return;

        if (position[index] != 255)
        {
            position[index]++;
            positionAsString = asString();
            return;
        }

        position[index] = 0;
        addAtPosition(--index);
    }
public:
    IPAddress(const short& pos1, const short& pos2, const short& pos3, const short& pos4)
    {
        init(pos1, pos2, pos3, pos4);
    }

    IPAddress(QStringView ip)
    {
        QVector<short> vec;
        foreach (QStringView b, ip.split('.'))
            vec.append(b.toShort());
        init(vec[0], vec[1], vec[2], vec[3]);
    }

    IPAddress next()
    {
        IPAddress address(position[0], position[1], position[2], position[3]);
        address.addAtPosition(3);
        return address;
    }

    friend bool operator==(const IPAddress& l, const IPAddress& r) { return l.positionAsString == r.positionAsString; }
    friend bool operator!=(const IPAddress& l, const IPAddress& r) { return !(l == r); }
    friend bool operator<(const IPAddress& l, const IPAddress& r) { return l.positionAsString.compare(r.positionAsString) < 0; }
    friend bool operator>(const IPAddress& l, const IPAddress& r) { return r < l; }
    friend bool operator<=(const IPAddress& l, const IPAddress& r) { return !(l > r); }
};

#endif // IPADDRESS_H
