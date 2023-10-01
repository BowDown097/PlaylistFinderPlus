#ifndef PLAYLISTPARSER_H
#define PLAYLISTPARSER_H
#include "parseresult.h"
#include <QRegularExpression>

namespace PlaylistParser
{
    static constexpr QLatin1StringView ipMask("$ip$");
    static constexpr QLatin1StringView portMask("$port$");

    static inline const QRegularExpression ipRegex("\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\b");
    static inline const QRegularExpression portRegex("\\d{2,5}");
    static inline const QRegularExpression proxyIpRegex("://" + ipRegex.pattern());

    ParseResult parsePlaylist(const QStringList& lines);
}

#endif // PLAYLISTPARSER_H
