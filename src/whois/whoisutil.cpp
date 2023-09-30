#include "whoisutil.h"
#include <cmath>

Range getRange(const QString& ipAndSubnetMask)
{
    QStringList split = ipAndSubnetMask.split('/');
    IpAddress ip(split[0]);
    IpAddress subnetMask = getSubnetMask(split[1].toInt());

    QList<ushort> ipBytes = ip.addressBytes();
    QList<ushort> subnetBytes = subnetMask.addressBytes();

    if (ipBytes.size() != subnetBytes.size())
        throw std::invalid_argument("Lengths of IP address and subnet mask do not match.");

    QList<ushort> ipBytes2;
    ipBytes2.reserve(ipBytes.size());
    for (int i = 0; i < ipBytes2.size(); i++)
        ipBytes2[i] = ipBytes[i] | (subnetBytes[i] ^ 255);

    return Range(ip, IpAddress(ipBytes2[0], ipBytes2[1], ipBytes2[2], ipBytes2[3]));
}

IpAddress getSubnetMask(int hostpartLength)
{
    if (hostpartLength < 2)
        throw std::invalid_argument("Number of hosts is too little for IPv4");
    else if (hostpartLength > 32)
        throw std::invalid_argument("Number of hosts is too large for IPv4");

    ushort bytes[4];
    for (int i = 0; i < 4; i++)
    {
        if (i * 8 + 8 <= hostpartLength)
            bytes[i] = 255;
        else if (i * 8 > hostpartLength)
            bytes[i] = 0;
        else
            bytes[i] = 256 - pow(2, 8 - (hostpartLength - i * 8));
    }

    return IpAddress(bytes[0], bytes[1], bytes[2], bytes[3]);
}
