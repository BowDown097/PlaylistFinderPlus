#ifndef SCANNER_H
#define SCANNER_H
#include <numeric>
#include <scanner/httpheaders.h>
#include <scanner/util.h>
#include <QCoreApplication>
#include <QPair>
#include <QThread>

class Scanner
{
    int completions, searchId;
    HTTPHeaders headers;
    bool paused, stopped;

    void processRange(int total, QVector<QPair<IPAddress, ushort>> range, int maxConnections) {}
public:
    void findProxies(const QStringList& rangeLines, const QStringList& portLines, int maxConnections)
    {
        completions = 0;
        searchId++;
        maxConnections *= 10;
        paused = stopped = false;

        QVector<ushort> ports = getAllPorts(portLines);
        QVector<QVector<IPAddress>> ranges = getAllRanges(rangeLines);

        int total = 0;
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
                    processRange(total, fullIps, maxConnections);
                    fullIps.clear();
                }
            }
        }

        processRange(total, fullIps, maxConnections);
    }
};

#endif // SCANNER_H
