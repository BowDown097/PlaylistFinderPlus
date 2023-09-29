#ifndef PLAYLISTPARSER_H
#define PLAYLISTPARSER_H
#include "parseresult.h"
#include <QRegularExpression>

class PlaylistParser
{
public:
    ParseResult parsePlaylist(const QStringList& lines);

    QString ipPlaceholder() const { return ipMask; }
    QString portPlaceholder() const { return portMask; }
private:
    const QString ipMask = "$ip$";
    const QRegularExpression ipRegex = QRegularExpression("\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\b");
    const QString portMask = "$port$";
    const QRegularExpression portRegex = QRegularExpression("\\d{2,5}");
    const QRegularExpression proxyIpRegex = QRegularExpression("://" + ipRegex.pattern());
};

#endif // PLAYLISTPARSER_H
