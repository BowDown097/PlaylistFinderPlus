#ifndef SCANNER_H
#define SCANNER_H
#include "httpheaders.h"
#include "model/ipaddress.h"
#include <functional>

class Scanner : public QObject
{
    Q_OBJECT
public:
    friend class ScanTask;

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

    void processRange(int total, const QList<QPair<IpAddress, ushort>>& range, int maxConnections);
signals:
    void lastIpPort(const QString& ip, ushort port);
    void progress(int completions, int total);
    void newProxyFound(const QString& ip, ushort port, const QString& proxyType, const QString& server);
};

#endif // SCANNER_H
