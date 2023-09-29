#ifndef SCANRESULT_H
#define SCANRESULT_H
#include <QString>

class ScanResult
{
public:
    bool sel;
    QString info;
    QString ip;
    ushort port;

    ScanResult(bool sel, const QString& ip, const QString& info, ushort port)
        : sel(sel), info(info), ip(ip), port(port) {}
};

#endif // SCANRESULT_H
