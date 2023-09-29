#include "ddnsservice.h"
#include <QFile>

QList<Ddns> DdnsService::getDdnsList()
{
    QList<Ddns> out;

    QFile file("ddns.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return out;

    QList<QString> lines;
    while (file.canReadLine())
        lines.append(file.readLine());

    for (const QString& line : lines)
    {
        int ind = line.indexOf("=");
        if (ind == -1)
            continue;

        QString name = line.left(ind).trimmed();
        QString url = line.mid(ind + 1).trimmed();
        out.append(Ddns(name, url));
    }

    return out;
}
