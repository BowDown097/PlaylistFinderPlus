#ifndef SCANNER_H
#define SCANNER_H
#include "model/ipaddress.h"
#include "scanner/httpheaders.h"

class Scanner
{
public:
    void findProxies(const QStringList& rangeLines, const QStringList& portLines, int maxConnections);
private:
    int completions;
    HTTPHeaders headers;
    bool paused;
    int searchId;
    bool stopped;

    void processRange([[maybe_unused]] int total, [[maybe_unused]] QList<QPair<IPAddress, ushort>> range,
                      [[maybe_unused]] int maxConnections)
    {}
};

#endif // SCANNER_H
