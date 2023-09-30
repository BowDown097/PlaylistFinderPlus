#include "scanner.h"
#include "util.h"
#include <QtConcurrent>

void Scanner::checkIp(const QString& ip, ushort port, int total)
{
    std::unique_ptr<QSslSocket> socket = std::make_unique<QSslSocket>();
    socket->connectToHostEncrypted(ip, port);
    if (socket->waitForEncrypted(1000))
    {
        QString proxyType = getProxyType(ip, port);
        setHttpHeaders(ip, port, QStringLiteral("http://%1:%2").arg(ip).arg(port));

        QString server;
        m_headers.tryGetHeader(ip, port, "SERVER", server);
        emit newProxyFound(ip, port, proxyType, server);
    }

    if (m_completions % 50 == 0 && m_completions > total / 100)
        emit progress(m_completions, total);

    m_completions++;
}

void Scanner::findProxies(const QString& rangeLines, const QString& portLines, int maxConnections,
                          const std::function<QString(const QString&, int, const QString&)>& getChannelUrl)
{
    m_completions = 0;
    m_searchId++;
    m_paused = m_stopped = false;
    m_getChannelUrl = getChannelUrl;
    maxConnections *= 10;
    m_headers.clear();

    QSet<ushort> ports;
    for (const QString& portLine : portLines.split('\n', Qt::SkipEmptyParts))
        for (ushort port : getAllPorts(portLine))
            ports.insert(port);
    
    QList<QList<IpAddress>> ranges;
    for (const QString& rangeLine : rangeLines.split('\n', Qt::SkipEmptyParts))
        ranges.append(getAllRanges(rangeLine));
    
    int total = std::accumulate(ranges.cbegin(), ranges.cend(), 0, [](int x, const QList<IpAddress>& y) {
        return x + y.size();
    }) * ports.size();
    
    QList<QPair<IpAddress, ushort>> ipsPorts;
    for (ushort port : ports)
    {
        for (const QList<IpAddress>& range : ranges)
        {
            for (const IpAddress& ip : range)
                ipsPorts.append(qMakePair(ip, port));

            if (ipsPorts.size() > maxConnections * 25)
            {
                processRange(total, ipsPorts, maxConnections);
                ipsPorts.clear();
            }
        }
    }

    processRange(total, ipsPorts, maxConnections);
}

QByteArray Scanner::getHttpData(const QString& ip, ushort port, const QString& urlAddress, int byteCount)
{
    if (m_stopped)
        return QByteArray();

    QNetworkRequest req((QUrl(urlAddress)));
    req.setRawHeader("User-Agent", "Mozilla");
    req.setTransferTimeout(5000);

    QNetworkReply* reply = m_networkManager->get(req);

    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    QByteArray out = reply->read(byteCount);
    m_headers.add(ip, port, reply->rawHeaderPairs());
    reply->deleteLater();
    return out;
}

int Scanner::getNumberOfClients(const QString& statusData)
{
    static QRegularExpression newlineRegex("[\r\n]+");
    QStringList split = statusData.split(newlineRegex, Qt::SkipEmptyParts);
    int num = 0;
    bool flag = false;

    for (const QString& text : split)
    {
        if (!flag)
            flag = text.startsWith("<h3>Active", Qt::CaseInsensitive);
        else if (text.startsWith("<tr><td>", Qt::CaseInsensitive) || text.startsWith("<tr class=\"uneven\">", Qt::CaseInsensitive))
            num++;
        else if (text.startsWith("<h3>", Qt::CaseInsensitive))
            break;
    }

    return num;
}

QString Scanner::getProxyType(const QString& ip, ushort port)
{
    if (port == 8001 || port == 8002)
    {
        QString ipUrl = "http://" + ip;
        QByteArray data = getHttpData(ip, 80, ipUrl, 5120).toUpper();
        if (data.contains("OPENWEBIF") || data.contains("DREAMBOX"))
            return "e2";
    }

    QString statusUrl = QStringLiteral("http://%1:%2/status").arg(ip).arg(port);
    QByteArray statusData = getHttpData(ip, port, statusUrl, 5120);
    if (statusData.contains("udpxy"))
    {
        int numberOfClients = getNumberOfClients(statusData);
        return numberOfClients > 20 ? "udpxy (>20)" : QStringLiteral("udpxy (%1)").arg(numberOfClients);
    }
    else if (statusData.contains("PROXY message : New thread handle is"))
    {
        return "udp-to-http";
    }
    else if (statusData.contains("total packets="))
    {
        setHttpHeaders(ip, port, QStringLiteral("http://%1:%2").arg(ip).arg(port));
        QString server;
        if (m_headers.tryGetHeader(ip, port, "SERVER", server))
        {
            server = server.toUpper();
            if (server.contains("MARTIN-VIDEO-PROXY"))
                return "martin";
            else if (server.contains("PGWEBSERV"))
                return "status";
        }
    }

    QString statUrl = QStringLiteral("http://%1:%2/stat").arg(ip).arg(port);
    QString statData = getHttpData(ip, port, statUrl, 5120);
    if (statData.contains("Astra "))
    {
        int disconnectCount = statData.split(">Disconnect</a></td></tr>", Qt::SkipEmptyParts).size() - 1;
        return disconnectCount > 20 ? "astra (>20)" : QStringLiteral("astra (%1)").arg(disconnectCount);
    }
    else if (statData.contains("Multicast to http proxy"))
    {
        static QRegularExpression newlineRegex("[\r\n]+");
        QStringList split = statData.split(newlineRegex, Qt::SkipEmptyParts);
        auto splitIt = std::ranges::find_if(split, [](const QString& x) { return x.startsWith("Total clients count:"); });
        if (splitIt != split.end())
        {
            QString totalClientsStr = splitIt->replace("Total clients count:", QString()).trimmed();
            return QStringLiteral("msd (%1)").arg(totalClientsStr.toInt());
        }
    }
    else if (statData.contains("nginx_rtmp_version"))
    {
        return "rtmp";
    }

    QString channelUrl = m_getChannelUrl(ip, port, QString());
    setHttpHeaders(ip, port, channelUrl);

    QString contentType, server;

    if (m_headers.tryGetHeader(ip, port, "SERVER", server))
    {
        server = server.toUpper();
        if (server.contains("ENIGMA2") || ((port == 8001 || port == 8002) && server.contains("STREAMSERVER")))
            return "e2";
        else if (server.contains("ASTRA"))
            return "astra";
        else if (server.contains("XPROXY"))
            return "xProxy";
        else if (server.contains("UDPXY"))
            return "udpxy";
        else if (server.contains("TRANSUDP"))
            return "transudp";
        else if (server.contains("UDPPROXY"))
            return "udp-to-http";
        else if (server.contains("MULTICAST TO HTTP PROXY"))
            return "msd";
        else if (server.contains("GWS") && server.contains("(FRESH)"))
            return "gws";
        else if (server.contains("MARTIN-VIDEO-PROXY"))
            return "martin";
        else if (server.contains("TVHEADEND"))
            return "tvheadend";
        else if (server.contains("TSBOX"))
            return "tsbox";
        else if (server.contains("MUMUDVB"))
            return "MuMuDVB";
    }

    if (m_headers.tryGetHeader(ip, port, "CONTENT-TYPE", contentType))
    {
        contentType = contentType.toUpper();
        if (contentType.contains("APPLICATION/VND.APPLE.MPEGURL"))
            return "apple";
        else if (contentType.contains("VIDEO"))
            return "+";
        else if (contentType.contains("STREAM"))
            return "-";
        else if (contentType.contains("TEXT") || contentType.contains("HTML") || contentType.contains("XML") || contentType.contains("json"))
            return QString();
    }

    QByteArray channelData = getHttpData(ip, port, channelUrl, 10240);
    if (!channelData.isEmpty() && channelData.length() == 10240)
    {
        channelData = channelData.toUpper();
        if (channelData.contains("<HEAD") && !channelData.contains("<BODY") && !channelData.contains("<DIV") &&
            !channelData.contains("<SPAN"))
        {
            return "?";
        }
    }

    return QString();
}

void Scanner::processRange(int total, const QList<QPair<IpAddress, ushort>>& range, int maxConnections)
{
    bool flag = false;
    QList<QPair<IpAddress, ushort>>::const_iterator it = range.begin();

    QList<QFuture<void>> tasks;
    while (!m_stopped)
    {
        if (m_paused)
        {
            QThread::sleep(1);
            flag = true;
        }
        else if (tasks.size() > maxConnections)
        {
            QThread::msleep(100);
            emit lastIpPort(QString(it->first), it->second);
            tasks.removeIf([](const QFuture<void>& future) { return future.isFinished() || future.isCanceled(); });
            QThread::msleep(900);
        }
        else if ((flag = (it != range.end())))
        {
            tasks.append(QtConcurrent::run(&Scanner::checkIp, this, QString(it->first), it->second, total));
            ++it;
        }

        if (!flag)
        {
            std::ranges::for_each(tasks, [](QFuture<void>& future) { future.waitForFinished(); });
            m_headers.clear();
            return;
        }
    }
}

void Scanner::setHttpHeaders(const QString& ip, ushort port, const QString& url)
{
    if (m_stopped || m_headers.contains(ip, port))
        return;

    QNetworkRequest req((QUrl(url)));
    req.setRawHeader("User-Agent", "Mozilla");
    req.setTransferTimeout(5000);

    QNetworkReply* reply = m_networkManager->get(req);

    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    m_headers.add(ip, port, reply->rawHeaderPairs());
    reply->deleteLater();
}
