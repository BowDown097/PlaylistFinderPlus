#ifndef UTIL_H
#define UTIL_H
#include "model/ipaddress.h"
#include <QStringList>

QList<IPAddress> getRange(const QString& from, const QString& to);
QList<ushort> getAllPorts(const QStringList& portLines);
QList<QList<IPAddress>> getAllRanges(const QStringList& rangeLines);

#endif // UTIL_H
