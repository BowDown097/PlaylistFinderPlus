#ifndef PLAYLISTPARSER_H
#define PLAYLISTPARSER_H
#include <QRegExp>
#include <QString>

class PlaylistParser
{
    const QRegExp ipRegex = QRegExp("\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\b");
    const QRegExp portRegex = QRegExp("\\d{2,5}");
    const QRegExp proxyRegex = QRegExp(QStringLiteral("://%1").arg(ipRegex.pattern()));
public:

};

#endif // PLAYLISTPARSER_H
