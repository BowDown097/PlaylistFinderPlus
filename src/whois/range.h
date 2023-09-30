#ifndef RANGE_H
#define RANGE_H
#include "model/ipaddress.h"

struct Range
{
    IpAddress from;
    IpAddress to;

    Range(const IpAddress& from, const IpAddress& to) : from(from), to(to) {}

    operator QString() const { return QString(from) + " - " + QString(to); }
    friend bool operator==(const Range& l, const Range& r) { return l.from == r.from && l.to == r.to; }
};

#endif // RANGE_H
