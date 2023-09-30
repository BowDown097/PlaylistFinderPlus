#ifndef WHOISUTIL_H
#define WHOISUTIL_H
#include "range.h"

Range getRange(const QString& ipAndSubnetMask);
IpAddress getSubnetMask(int hostpartLength);

#endif // WHOISUTIL_H
