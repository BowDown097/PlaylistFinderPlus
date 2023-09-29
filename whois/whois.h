#ifndef WHOIS_H
#define WHOIS_H
#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <whois/ipinfo.h>

class Whois
{
public:
    IPInfo getIpInfo(QNetworkAccessManager* manager, const QString& ip, ushort port)
    {
        QString data = "<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\"><s:Body><GetData xmlns=\"http://iptv-backend.apphb.com/\" xmlns:i=\"http://www.w3.org/2001/XMLSchema-instance\"><ip>" + ip +"</ip><port>" + QString::number(port) + "</port><source>pf1.18</source></GetData></s:Body></s:Envelope>";
        QNetworkRequest request(QUrl("http://iptv-backend.apphb.com/RangesAndPorts.asmx"));
        request.setRawHeader("Content-Type", "text/xml; charset=utf-8");
        request.setRawHeader("SOAPAction", "http://iptv-backend.apphb.com/GetData");
        request.setRawHeader("Host", "iptv-backend.apphb.com");
        request.setRawHeader("Content-Length", QByteArray::number(data.length()));
        request.setRawHeader("Expect", "100-continue");
        request.setRawHeader("Connection", "Keep-Alive");
        QNetworkReply* response = manager->post(request, data.toUtf8());
        QEventLoop event;
        QObject::connect(response, SIGNAL(finished()), &event, SLOT(quit()));
        event.exec();

        QString html = response->readAll();
    }
};

#endif // WHOIS_H
