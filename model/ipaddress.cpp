#include "ipaddress.h"

IPAddress::IPAddress(const QString& ip)
{
    QStringList split = ip.split('.');

    QList<short> bits;
    bits.reserve(split.size());

    std::ranges::transform(split, std::back_inserter(bits), [](const QString& s) { return s.toShort(); });

    init(bits[0], bits[1], bits[2], bits[3]);
}

void IPAddress::addAtPosition(ushort index)
{
    if (position[index] != 255)
    {
        position[index]++;
        positionAsString = asString();
        return;
    }

    if (index == 0)
        throw std::runtime_error("IP cannot be larger than 255.255.255.255");

    position[index] = 0;
    addAtPosition(--index);
}

QString IPAddress::asString() const
{
    return QString::asprintf("%03hu%03hu%03hu%03hu", position[0], position[1], position[2], position[3]);
}

void IPAddress::init(ushort pos1, ushort pos2, ushort pos3, ushort pos4)
{
    position.append({pos1, pos2, pos3, pos4});
    positionAsString = asString();
}

IPAddress IPAddress::next()
{
    IPAddress address(position[0], position[1], position[2], position[3]);
    address.addAtPosition(3);
    return address;
}

IPAddress::operator QString() const
{
    QStringList strings;
    strings.reserve(position.size());
    std::ranges::transform(position, std::back_inserter(strings), [](int i) { return QString::number(i); });
    return strings.join('.');
}
