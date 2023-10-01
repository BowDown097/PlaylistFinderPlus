#include "scantask.h"
#include "scanutil.h"
#include "task/taskexecutor.h"
#include <QFuture>

void Scanner::findProxies(const QString& rangeLines, const QString& portLines, int maxConnections,
                          std::function<QString(const QString&, ushort)> getChannelUrl)
{
    m_completions = 0;
    m_searchId++;
    m_paused = m_stopped = false;
    m_getChannelUrl = getChannelUrl;
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
            tasks.append(TaskExecutor::run(new ScanTask(this, QString(it->first), it->second, total)));
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
