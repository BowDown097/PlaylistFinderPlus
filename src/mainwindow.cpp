#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui/scantablemodel.h"
#include <QFileDialog>

// TODO:
// MainWindow: All button stuff (loading, searching, etc.)
// Scanner: httpheaders.h, scanner.h

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), scanner(new Scanner(this)), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // create table
    ScanTableModel* tableModel = new ScanTableModel(ui->tableView);
    ui->tableView->setModel(tableModel);
    ui->tableView->setColumnWidth(0, 25);
    // set up buttons
    connect(ui->playlistButton, &QPushButton::clicked, this, &MainWindow::loadPlaylist);
    connect(ui->searchButton, &QPushButton::clicked, this, &MainWindow::startSearch);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadPlaylist()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "Choose playlist file",
        QString(),
        "m3u (*.m3u *.m3u8);;All files (*.*)"
    );

    ui->playlistText->setText(fileName);
    ui->searchButton->setEnabled(true);

    QFile playlistFile(fileName);
    if (!playlistFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    static QRegularExpression newlineRegex("[\r\n]+");
    QStringList lines = QString(playlistFile.readAll()).split(newlineRegex);
    ParseResult parseResult = playlistParser.parsePlaylist(lines);
}

void MainWindow::startSearch()
{
    ui->pauseButton->setEnabled(true);
    ui->saveButton->setEnabled(true);
    ui->stopButton->setEnabled(true);
    ui->connectSpin->setEnabled(false);
    ui->playlistButton->setEnabled(false);
    ui->playlistText->setEnabled(false);
    ui->portsText->setEnabled(false);
    ui->rangesText->setEnabled(false);
    ui->searchButton->setEnabled(false);
}
