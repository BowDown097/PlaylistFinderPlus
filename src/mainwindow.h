#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "ddns/ddnsservice.h"
#include "playlistparser/playlistparser.h"
#include "scanner/scanner.h"
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
    void loadPlaylist();
    void startSearch();
private:
    QList<Ddns> ddnsList;
    DdnsService ddnsService;
    int maxConnections = 400;
    int maxProxies = 1000;
    PlaylistParser playlistParser;
    Scanner* scanner;
    Ddns selectedDdns;
    Ui::MainWindow* ui;
};
#endif // MAINWINDOW_H
