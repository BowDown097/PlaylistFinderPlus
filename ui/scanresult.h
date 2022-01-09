#ifndef SCANRESULT_H
#define SCANRESULT_H
#include <QString>

class ScanResult
{
public:
    bool sel;
    QString ip, info;
    ushort port;

    ScanResult(bool _sel, const QString& _ip, const QString& _info, ushort _port) : sel(_sel), ip(_ip), info(_info), port(_port) {}
};

#endif // SCANRESULT_H
