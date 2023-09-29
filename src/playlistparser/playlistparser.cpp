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

    QList<QString> split = ipPortMatch.captured().split(':');
    int port = 0;
    bool parseSuccess = false;
    if (!split.isEmpty())
        port = split[1].toInt(&parseSuccess);

    QString join = lines.join('\n');

    if (parseSuccess)
    {
        QString ipPort = QStringLiteral("%1:%2").arg(ipCapture).arg(port);
        QString ipPortMask = ipMask + ":" + portMask;
        join.replace(ipPort, ipPortMask);
        firstLink.replace(ipPort, ipPortMask);
    }

    join.replace(ipCapture, ipMask);
    firstLink.replace(ipCapture, ipMask);

    return ParseResult(ipCapture, parseSuccess ? port : 80, join, firstLink);
}
