#include "playlistparser.h"

ParseResult PlaylistParser::parsePlaylist(const QStringList& lines)
{
    auto firstLinkIter = std::ranges::find_if(lines, [](const QString& line) {
        QString trimmed = line.trimmed();
        return trimmed.startsWith("http") || trimmed.startsWith("rtmp");
    });

    if (firstLinkIter == lines.end())
        return ParseResult();

    QString firstLink = *firstLinkIter;
    QRegularExpressionMatch ipMatch = ipRegex.match(proxyIpRegex.match(firstLink).captured());
    if (!ipMatch.hasMatch())
        return ParseResult();

    QString ipCapture = ipMatch.captured();
    QRegularExpression ipPortRegex(ipCapture + ":" + portRegex.pattern());
    QRegularExpressionMatch ipPortMatch = ipPortRegex.match(firstLink);
    if (!ipPortMatch.hasMatch())
        return ParseResult();

    QStringList split = ipPortMatch.captured().split(':');
    ushort port = 0;
    bool parseSuccess = false;
    if (split.size() > 1)
        port = split[1].toUShort(&parseSuccess);

    QString join = lines.join('\n');
    QString host = parseSuccess ? QStringLiteral("%1:%2").arg(ipCapture).arg(port) : ipCapture;
    QString hostMask = ipMask + ":" + portMask;

    join.replace(host, hostMask).replace(ipCapture, ipMask);
    firstLink.replace(host, hostMask).replace(ipCapture, ipMask);

    return ParseResult(ipCapture, parseSuccess ? port : 80, join, firstLink);
}
