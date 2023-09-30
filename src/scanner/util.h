#ifndef UTIL_H
#define UTIL_H
#include "model/ipaddress.h"

QList<ushort> getAllPorts(const QString& ports);
QList<IPAddress> getAllRanges(const QString& range);
QList<IPAddress> getRange(const QString& from, const QString& to);

#endif // UTIL_H
