#ifndef SCANTASK_H
#define SCANTASK_H
#include "scanner.h"
#include "task/controllabletask.h"
#include <QNetworkAccessManager>
#include <QRegularExpression>

class ScanTask : public ControllableTask<void>
{
public:
    ScanTask(Scanner* parent, const QString& ip, ushort port, int total)
        : ControllableTask<void>(),
          ip(ip),
          networkManager(new QNetworkAccessManager),
          port(port),
          scanner(parent),
          total(total)
    {}

    ~ScanTask() override;
    void run(const TaskControl&) override;
private:
    static inline const QRegularExpression newlineRegex = QRegularExpression("[\r\n]+");
    static inline const QString stop = "STOP_RUNNING_TASK";

    QString ip;
    QNetworkAccessManager* networkManager;
    ushort port;
    Scanner* scanner;
    int total;

    QByteArray getHttpData(const QString& ip, ushort port, const QString& urlAddress, int byteCount);
    int getNumberOfClients(const QString& statusData);
    QString getProxyType(const QString& ip, ushort port);
    void setHttpHeaders(const QString& ip, ushort port, const QString& url);
};

#endif // SCANTASK_H
