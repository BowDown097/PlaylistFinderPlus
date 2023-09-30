#ifndef WHOISRESULT_H
#define WHOISRESULT_H
#include <QStringList>

struct WhoisResult
{
    QString country;
    QString network;
    QStringList ranges;
};

#endif // WHOISRESULT_H
