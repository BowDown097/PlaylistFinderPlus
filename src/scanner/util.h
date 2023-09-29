#ifndef UTIL_H
#define UTIL_H
#include "model/ipaddress.h"
#include <QStringList>

QList<ushort> getAllPorts(const QStringList& portLines);
QList<QList<IPAddress>> getAllRanges(const QStringList& rangeLines);
QList<IPAddress> getRange(const QString& from, const QString& to);

#endif // UTIL_H
