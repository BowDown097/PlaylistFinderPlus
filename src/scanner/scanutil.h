#ifndef SCANUTIL_H
#define SCANUTIL_H
#include "model/ipaddress.h"

QList<ushort> getAllPorts(const QString& ports);
QList<IpAddress> getAllRanges(const QString& range);
QList<IpAddress> getRange(const QString& from, const QString& to);

#endif // SCANUTIL_H
