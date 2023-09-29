#include "ipaddress.h"

IPAddress::IPAddress(const QString& ip)
{
    QStringList split = ip.split('.');

    QList<short> vec;
    vec.reserve(split.size());

    std::ranges::transform(split, std::back_inserter(vec), [](const QString& s) { return s.toShort(); });

    init(vec[0], vec[1], vec[2], vec[3]);
}

void IPAddress::addAtPosition(ushort index)
{
    if (index == 0)
        return;

    if (position[index] != 255)
    {
        position[index]++;
        return;
    }

    position[index] = 0;
    addAtPosition(--index);
}

void IPAddress::init(ushort pos1, ushort pos2, ushort pos3, ushort pos4)
{
    position.append({pos1, pos2, pos3, pos4});
}

IPAddress IPAddress::next()
{
    IPAddress address(position[0], position[1], position[2], position[3]);
    address.addAtPosition(3);
    return address;
}
