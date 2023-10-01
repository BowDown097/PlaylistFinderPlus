#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "playlistparser/playlistparser.h"
#include "scanner/scanner.h"
#include "ui/scantablemodel.h"
#include "whois/whoisclient.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
private slots:
    void addProxy(const QString& ip, ushort port, const QString& proxyType, const QString& server);
    void loadPlaylist();
    void multipleSavePlaylist();
    void searchCompleted();
    void startSearch();
    void stopSearch();
    void togglePauseSearch();
    void updateLastIpPort(const QString& ip, ushort port);
    void updateProgress(int completions, int total);
private:
    QString lastIpPort;
    QString maskedLine;
    QString maskedPlaylist;
    QString playlistPath;

    PlaylistParser playlistParser;
    Scanner* scanner;
    ScanTableModel* tableModel;
    Ui::MainWindow* ui;
    WhoisClient* whoisClient;

    QString replaceIpAndPort(QString str, const QString& newIp, ushort newPort);
};
#endif // MAINWINDOW_H
