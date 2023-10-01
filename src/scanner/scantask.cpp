#include "scantask.h"
#include <QEventLoop>

QByteArray ScanTask::getHttpData(const QString& ip, ushort port, const QString& urlAddress, int byteCount)
{
    QNetworkRequest req((QUrl(urlAddress)));
    req.setRawHeader("User-Agent", "Mozilla");
    req.setTransferTimeout(5000);

    QNetworkReply* reply = networkManager->get(req);

    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    QByteArray out = reply->read(byteCount);
    scanner->m_headers.add(ip, port, reply->rawHeaderPairs());
    reply->deleteLater();
    return out;
}

int ScanTask::getNumberOfClients(const QString& statusData)
{
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

QString ScanTask::getProxyType(const QString& ip, ushort port)
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
    if (scanner->m_stopped)
        return stop;

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
        if (scanner->m_headers.tryGetHeader(ip, port, "SERVER", server))
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
    if (scanner->m_stopped)
        return stop;

    if (statData.contains("Astra "))
    {
        int disconnectCount = statData.split(">Disconnect</a></td></tr>", Qt::SkipEmptyParts).size() - 1;
        return disconnectCount > 20 ? "astra (>20)" : QStringLiteral("astra (%1)").arg(disconnectCount);
    }
    else if (statData.contains("Multicast to http proxy"))
    {
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

    QString channelUrl = scanner->m_getChannelUrl(ip, port);
    setHttpHeaders(ip, port, channelUrl);

    QString contentType, server;

    if (scanner->m_headers.tryGetHeader(ip, port, "SERVER", server))
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

    if (scanner->m_headers.tryGetHeader(ip, port, "CONTENT-TYPE", contentType))
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

void ScanTask::run(const TaskControl&)
{
    std::unique_ptr<QTcpSocket> socket = std::make_unique<QTcpSocket>();
    socket->connectToHost(ip, port);

    if (!scanner->m_stopped && socket->waitForConnected(1000))
    {
        QString proxyType = getProxyType(ip, port);
        if (proxyType != stop)
        {
            setHttpHeaders(ip, port, QStringLiteral("http://%1:%2").arg(ip).arg(port));

            QString server;
            scanner->m_headers.tryGetHeader(ip, port, "SERVER", server);
            emit scanner->newProxyFound(ip, port, proxyType, server);
        }
    }

    scanner->m_completions++;
    emit scanner->progress(scanner->m_completions, total);
}

void ScanTask::setHttpHeaders(const QString& ip, ushort port, const QString& url)
{
    if (scanner->m_stopped || scanner->m_headers.contains(ip, port))
        return;

    QNetworkRequest req((QUrl(url)));
    req.setRawHeader("User-Agent", "Mozilla");
    req.setTransferTimeout(5000);

    QNetworkReply* reply = networkManager->get(req);

    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    scanner->m_headers.add(ip, port, reply->rawHeaderPairs());
    reply->deleteLater();
}

ScanTask::~ScanTask()
{
    networkManager->deleteLater();
}
