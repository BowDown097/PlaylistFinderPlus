#ifndef SCANNER_H
#define SCANNER_H
#include <QPair>
#include <numeric>
#include "util.h"

class Scanner
{
    int completions;
    int memberSearchId;
    bool stopped;
public:
    bool paused;

    Scanner() {}

    void findProxies(const QStringList& rangeLines, const QStringList& portLines, int maxConnections)
    {
        completions = 0;
        memberSearchId++;
        maxConnections *= 10;
        paused = stopped = false;

        QVector<ushort> ports = getAllPorts(portLines);
        QVector<QVector<IPAddress>> ranges = getAllRanges(rangeLines);

        int total;
        foreach (const QVector<IPAddress>& range, ranges)
            total += range.size();
        total *= ports.size();

        QVector<QPair<IPAddress, ushort>> fullIps;
        foreach (ushort port, ports)
        {
            foreach (const QVector<IPAddress>& range, ranges)
            {
                foreach (const IPAddress& ip, range)
                    fullIps.append(QPair<IPAddress, ushort>(ip, port));
                if (fullIps.size() > maxConnections * 25)
                {
                    // do funny here, i need to play minecraft
                }
            }
        }
    }
};

#endif // SCANNER_H
