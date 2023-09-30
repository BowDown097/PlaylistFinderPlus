#include "scanutil.h"

QList<ushort> getAllPorts(const QString& ports)
{
    QList<ushort> out;

    bool ok;
    ushort port;
    if (ports.contains('-'))
    {
        QStringList split = ports.split('-');
        bool ok2;
        port = split[0].toInt(&ok);
        ushort port2 = split[1].toInt(&ok2);

        if (ok && ok2)
        {
            out.resize(port2 - port + 1);
            std::iota(out.begin(), out.end(), port);
        }
    }
    else if (port = ports.toInt(&ok); ok)
    {
        out = { port };
    }

    return out;
}

QList<IpAddress> getAllRanges(const QString& range)
{
    if (range.contains('-'))
    {
        QStringList split = range.split('-');
        return getRange(split[0], split[1]);
    }

    return getRange(range, range);
}

QList<IpAddress> getRange(const QString& from, const QString& to)
{
    IpAddress fromAddress(from.trimmed());
    IpAddress toAddress(to.trimmed());
    
    QList<IpAddress> range;
    do
    {
        range.append(fromAddress);
        fromAddress = fromAddress.next();
    } while (fromAddress <= toAddress);

    return range;
}
