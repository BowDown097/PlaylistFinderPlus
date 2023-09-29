#include "util.h"

QList<IPAddress> getRange(const QString& from, const QString& to)
{
    QList<IPAddress> range;
    IPAddress addressTo(to);

    for (IPAddress addressFrom(from); addressFrom <= addressTo; addressFrom = addressFrom.next())
        range.append(addressFrom);

    return range;
}

QList<ushort> getAllPorts(const QStringList& portLines)
{
    QList<ushort> ports;

    for (const QString& portLine : portLines)
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
                QVector<ushort> portRange;
                portRange.reserve(port2 - port + 1);
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

QList<QList<IPAddress>> getAllRanges(const QStringList& rangeLines)
{
    QList<QList<IPAddress>> ranges;

    for (const QString& range : rangeLines)
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
