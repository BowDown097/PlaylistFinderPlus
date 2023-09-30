#ifndef SCANRESULT_H
#define SCANRESULT_H
#include <QString>

struct ScanResult
{
    bool sel;
    QString httpServer;
    QString info;
    QString ip;
    ushort port;

    ScanResult(const QString& ip, ushort port, const QString& info, const QString& httpServer)
        : sel(!info.isEmpty()), httpServer(httpServer), info(info), ip(ip), port(port) {}
};

#endif // SCANRESULT_H
