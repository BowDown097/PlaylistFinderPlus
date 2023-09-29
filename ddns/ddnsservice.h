#ifndef DDNSSERVICE_H
#define DDNSSERVICE_H
#include "ddns.h"
#include <QList>

class DdnsService
{
public:
    QList<Ddns> getDdnsList();
};

#endif // DDNSSERVICE_H
