#ifndef PARSERESULT_H
#define PARSERESULT_H
#include <QString>

class ParseResult
{
public:
    ParseResult() = default;
    ParseResult(const QString& proxyIp, int proxyPort, const QString& maskedPlaylist, const QString& maskedLine)
        : m_ip(proxyIp), m_maskedLine(maskedLine), m_maskedPlaylist(maskedPlaylist), m_port(proxyPort) {}

    QString ip() const { return m_ip; }
    QString maskedLine() const { return m_maskedLine; }
    QString maskedPlaylist() const { return m_maskedPlaylist; }
    int port() const { return m_port; }
private:
    QString m_ip;
    QString m_maskedLine;
    QString m_maskedPlaylist;
    int m_port;
};

#endif // PARSERESULT_H
