#ifndef UTIL_H
#define UTIL_H
#define ushort unsigned short
#include "model/ipaddress.h"
#include <QStringList>

QVector<IPAddress> getRange(QStringView from, QStringView to)
{
    QVector<short> fromVec, toVec;
    foreach (QStringView b, from.split('.'))
        fromVec.append(b.toShort());
    foreach (QStringView b, to.split('.'))
        toVec.append(b.toShort());
    QVector<IPAddress> range;
    IPAddress addressFrom(fromVec[0], fromVec[1], fromVec[2], fromVec[3]);
    IPAddress addressTo(toVec[0], toVec[1], toVec[2], toVec[3]);
    while (addressFrom <= addressTo)
    {
        range.append(addressFrom);
        addressFrom = addressFrom.next();
    }

    return range;
}

QVector<ushort> getAllPorts(const QStringList& portLines)
{
    QVector<ushort> ports{};
    foreach (QString portLine, portLines)
    {
        bool portParsed, port2Parsed;
        if (portLine.contains("-"))
        {
            QStringList split = portLine.split('-');
            ushort port = split[0].toUShort(&portParsed);
            ushort port2 = split[1].toUShort(&port2Parsed);
            if (portParsed && port2Parsed)
            {
                QVector<ushort> portRange(port2 - port + 1);
                std::iota(portRange.begin(), portRange.end(), port);
                ports.append(portRange);
            }
        }
        else
        {
            ushort port = portLine.toUShort(&portParsed);
            if (portParsed) ports.append(port);
        }
    }

    return ports;
}

QVector<QVector<IPAddress>> getAllRanges(const QStringList& rangeLines)
{
    QVector<QVector<IPAddress>> ranges;
    foreach (QString range, rangeLines)
    {
        if (range.contains('-'))
        {
            QStringList list = range.split('-');
            ranges.append(getRange(list[0], list[1]));
        }
        ranges.append(getRange(range, range));
    }
    return ranges;
}

#endif // UTIL_H
