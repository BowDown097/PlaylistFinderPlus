#ifndef SCANNER_H
#define SCANNER_H
#include "model/ipaddress.h"
#include "scanner/httpheaders.h"
#include <functional>
#include <QNetworkAccessManager>

class Scanner : public QObject
{
    Q_OBJECT
public:
    explicit Scanner(QObject* parent = nullptr) : QObject(parent) {}

    void findProxies(const QString& rangeLines, const QString& portLines, int maxConnections,
                     std::function<QString(const QString&, ushort)> getChannelUrl);

    void pause() { m_paused = !m_paused; }
    bool paused() const { return m_paused; }

    void stop() { m_stopped = true; }
private:
    int m_completions = 0;
    std::function<QString(const QString&, ushort)> m_getChannelUrl;
    HttpHeaders m_headers;
    bool m_paused = false;
    int m_searchId = 0;
    bool m_stopped = false;

    void checkIp(const QString& ip, ushort port, int total);
    QByteArray getHttpData(const QString& ip, ushort port, const QString& urlAddress, int byteCount);
    int getNumberOfClients(const QString& statusData);
    QString getProxyType(const QString& ip, ushort port);
    void processRange(int total, const QList<QPair<IpAddress, ushort>>& range, int maxConnections);
    void setHttpHeaders(const QString& ip, ushort port, const QString& url);
signals:
    void lastIpPort(const QString& ip, ushort port);
    void progress(int completions, int total);
    void newProxyFound(const QString& ip, ushort port, const QString& proxyType, const QString& server);
};

#endif // SCANNER_H
