#ifndef PLAYLISTPARSER_H
#define PLAYLISTPARSER_H
#include <QRegularExpression>

class PlaylistParser
{
private:
    const QRegularExpression ipRegex = QRegularExpression("\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\b");
    const QRegularExpression portRegex = QRegularExpression("\\d{2,5}");
    const QRegularExpression proxyRegex = QRegularExpression("://" + ipRegex.pattern());
};

#endif // PLAYLISTPARSER_H
