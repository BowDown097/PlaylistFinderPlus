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
    Scanner() : m_networkManager(new QNetworkAccessManager(this)) {}

    void findProxies(const QString& rangeLines, const QString& portLines, int maxConnections,
                     const std::function<QString(const QString&, int, const QString&)>& getChannelUrl);

    void pause() { m_paused = !m_paused; }
    bool paused() const { return m_paused; }

    void stop() { m_stopped = true; }
private:
    int m_completions = 0;
    std::function<QString(const QString&, int, const QString&)> m_getChannelUrl;
    HTTPHeaders m_headers;
    QNetworkAccessManager* m_networkManager;
    bool m_paused = false;
    int m_searchId = 0;
    bool m_stopped = false;

    void checkIp(const QString& ip, ushort port, int total);
    QByteArray getHttpData(const QString& ip, ushort port, const QString& urlAddress, int byteCount);
    int getNumberOfClients(const QString& statusData);
    QString getProxyType(const QString& ip, ushort port);
    void processRange(int total, const QList<QPair<IPAddress, ushort>>& range, int maxConnections);
    void setHttpHeaders(const QString& ip, ushort port, const QString& url);
signals:
    void lastIpPort(const QString& ip, ushort port);
    void progress(int completions, int total);
    void newProxyFound(const QString& ip, ushort port, const QString& proxyType, const QString& server);
};

#endif // SCANNER_H
