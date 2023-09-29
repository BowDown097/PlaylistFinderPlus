#include "scanner.h"
#include "util.h"

void Scanner::findProxies(const QStringList& rangeLines, const QStringList& portLines, int maxConnections)
{
    completions = 0;
    searchId++;
    maxConnections *= 10;
    paused = stopped = false;

    QList<ushort> ports = getAllPorts(portLines);
    QList<QList<IPAddress>> ranges = getAllRanges(rangeLines);

    int total = std::accumulate(ranges.cbegin(), ranges.cend(), 0, [](int x, const QList<IPAddress>& y) {
        return x + y.size();
    }) * ports.size();

    QList<QPair<IPAddress, ushort>> fullIps;
    for (ushort port : ports)
    {
        for (const QVector<IPAddress>& range : ranges)
        {
            for (const IPAddress& ip : range)
                fullIps.append(QPair<IPAddress, ushort>(ip, port));

            if (fullIps.size() > maxConnections * 25)
            {
                processRange(total, fullIps, maxConnections);
                fullIps.clear();
            }
        }
    }

    processRange(total, fullIps, maxConnections);
}
