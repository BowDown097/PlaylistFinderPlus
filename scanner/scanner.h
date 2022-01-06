#ifndef SCANNER_H
#define SCANNER_H
#include <numeric>
#include <QString>
#include <QStringList>
#include <QVector>

class Scanner
{
    int completions;
    int memberSearchId;
    bool stopped;

    QVector<int> getAllPorts(const QStringList& portLines)
    {
        QVector<int> ports{};
        foreach (QString portLine, portLines)
        {
            bool portParsed, port2Parsed;
            if (portLine.contains("-"))
            {
                QStringList split = portLine.split('-');
                const int port = split[0].toInt(&portParsed);
                const int port2 = split[1].toInt(&port2Parsed);
                if (portParsed && port2Parsed)
                {
                    QVector<int> portRange(port2 - port + 1);
                    std::iota(portRange.begin(), portRange.end(), port);
                    ports.append(portRange);
                }
            }
            else
            {
                const int port = portLine.toInt(&portParsed);
                if (portParsed) ports.append(port);
            }
        }

        return ports;
    }
public:
    bool paused;

    Scanner() {}

    void findProxies(const QStringList& ranges, const QStringList& portLines, int maxConnections)
    {
        completions = 0;
        memberSearchId++;
        maxConnections *= 10;
        paused = stopped = false;

        QVector<int> ports = getAllPorts(portLines);
    }
};

#endif // SCANNER_H
