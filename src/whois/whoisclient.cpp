#include "whoisclient.h"
#include <QEventLoop>
#include <QNetworkReply>
#include <QRegularExpression>
#include <QXmlStreamReader>

QString WhoisClient::findStringAndGetValue(const QStringList& lines, const QString& searchString)
{
    auto it = std::ranges::find_if(lines, [&searchString](const QString& x) { return x.startsWith(searchString); });
    if (it != lines.end())
    {
        QString decodedVal = QUrl::fromPercentEncoding(it->toUtf8());
        return decodedVal.replace(searchString, QString()).trimmed().toUpper();
    }

    return QString();
}

IpInfo WhoisClient::getIpInfo(const QString& ip, ushort port)
{
    QString mainRange;
    WhoisResult data = getWhoisData(ip, port);
    mainRange = data.ranges[0];
    data.ranges.remove(0);

    if (mainRange.isEmpty())
    {
        QString siteText = getSiteText("http://whoiz.herokuapp.com/lookup?url=" + ip);
        if (!siteText.isEmpty())
        {
            static QRegularExpression newlineRegex("[\r\n]+");
            const QStringList split = siteText.split(newlineRegex);
            std::ranges::for_each(split, [](const QString& x) { return x.trimmed().toLower(); });

            data.country = findStringAndGetValue(split, "country:");
            data.network = findStringAndGetValue(split, "org-name:");
            if (data.network.isEmpty())
                data.network = findStringAndGetValue(split, "netname:");

            QString descr = findStringAndGetValue(split, "descr:");
            if (!descr.isEmpty() && data.network != descr)
                data.network = QStringLiteral("%1 (%2)").arg(data.network, descr);

            mainRange = findStringAndGetValue(split, "inetnum:");
        }
    }

    return IpInfo(data.country, data.network, mainRange, data.ranges);
}

QByteArray WhoisClient::getSiteText(const QString& uri)
{
    QNetworkRequest req((QUrl(uri)));
    req.setRawHeader("User-Agent", "Mozilla");

    QNetworkReply* reply = m_networkManager->get(req);

    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    QByteArray out = reply->readAll();
    reply->deleteLater();
    return out;
}

WhoisResult WhoisClient::getWhoisData(const QString& ip, ushort port)
{
    QString data = QStringLiteral(R"(
        <s:Envelope xmlns:s="http://schemas.xmlsoap.org/soap/envelope/">
            <s:Body>
                <GetData xmlns="http://iptv-backend.apphb.com/" xmlns:i="http://www.w3.org/2001/XMLSchema-instance">
                    <ip>%1</ip>
                    <port>%2</port>
                    <source>pf1.20</source>
                </GetData>
            </s:Body>
        </s:Envelope>
    )").arg(ip).arg(port);

    QNetworkRequest req((QUrl("https://pfbackend.bsite.net/RangesAndPorts.asmx")));
    req.setRawHeader("Content-Type", "text/xml; charset=utf-8");
    req.setRawHeader("SOAPAction", "http://iptv-backend.apphb.com/GetData");
    req.setRawHeader("Host", "iptv-backend.apphb.com");
    req.setRawHeader("Content-Length", QByteArray::number(data.length()));
    req.setRawHeader("Expect", "100-continue");
    req.setRawHeader("Connection", "keep-alive");

    QNetworkReply* reply = m_networkManager->post(req, data.toUtf8());

    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    QByteArray response = reply->readAll();
    reply->deleteLater();

    WhoisResult result;
    if (!response.isEmpty())
    {
        QXmlStreamReader xml(response);
        while (!xml.atEnd())
        {
            xml.readNext();
            if (!xml.isStartElement())
                continue;

            if (xml.name().compare("Ranges") == 0)
            {
                xml.readNext();
                while (xml.name().compare("Ranges") != 0)
                {
                    result.ranges.append(xml.readElementText());
                    xml.readNextStartElement();
                }
            }
            else if (xml.name().compare("Country") == 0)
            {
                result.country = xml.readElementText();
            }
            else if (xml.name().compare("Network") == 0)
            {
                result.network = xml.readElementText();
            }
        }

        if (xml.hasError())
            qCritical().noquote() << "Error parsing whois XML:" << xml.errorString();
    }

    return result;
}
