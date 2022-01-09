#ifndef UTIL_H
#define UTIL_H
#include <model/ipaddress.h>
#include <QStringList>

QVector<IPAddress> getRange(const QString& from, const QString& to)
{
    QVector<IPAddress> range;
    IPAddress addressTo(to);
    for (IPAddress addressFrom(from); addressFrom <= addressTo; addressFrom = addressFrom.next())
        range.append(addressFrom);
    return range;
}

QVector<ushort> getAllPorts(const QStringList& portLines)
{
    QVector<ushort> ports;
    foreach (const QString& portLine, portLines)
    {
        bool portParsed, port2Parsed;
        ushort port, port2;
        if (portLine.contains("-"))
        {
            QStringList split = portLine.split('-');
            port = split[0].toUShort(&portParsed);
            port2 = split[1].toUShort(&port2Parsed);
            if (portParsed && port2Parsed)
            {
                QVector<ushort> portRange(port2 - port + 1);
                std::iota(portRange.begin(), portRange.end(), port);
                ports.append(portRange);
            }
        }
        else
        {
            port = portLine.toUShort(&portParsed);
            if (portParsed)
                ports.append(port);
        }
    }

    return ports;
}

QVector<QVector<IPAddress>> getAllRanges(const QStringList& rangeLines)
{
    QVector<QVector<IPAddress>> ranges;
    foreach (const QString& range, rangeLines)
    {
        if (range.contains('-'))
        {
            QStringList list = range.split('-');
            ranges.append(getRange(list[0], list[1]));
        }
        else
        {
            ranges.append(getRange(range, range));
        }
    }
    return ranges;
}

#endif // UTIL_H
