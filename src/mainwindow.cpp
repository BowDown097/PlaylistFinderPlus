#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QtConcurrent>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      scanner(new Scanner(this)),
      ui(new Ui::MainWindow),
      whoisClient(new WhoisClient(this))
{
    setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);
    // create table
    tableModel = new ScanTableModel(ui->tableView);
    ui->tableView->setModel(tableModel);
    ui->tableView->setColumnWidth(0, 25);
    // set up buttons
    connect(ui->pauseButton, &QPushButton::clicked, this, std::bind(&Scanner::pause, scanner));
    connect(ui->playlistButton, &QPushButton::clicked, this, &MainWindow::loadPlaylist);
    connect(ui->searchButton, &QPushButton::clicked, this, &MainWindow::startSearch);
    connect(ui->stopButton, &QPushButton::clicked, this, std::bind(&Scanner::stop, scanner));
    // set up scanner
    connect(scanner, &Scanner::lastIpPort, this, &MainWindow::updateLastIpPort);
    connect(scanner, &Scanner::newProxyFound, this, &MainWindow::addProxy);
    connect(scanner, &Scanner::progress, this, &MainWindow::updateProgress);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addProxy(const QString& ip, ushort port, const QString& proxyType, const QString& server)
{
    tableModel->append(ScanResult(ip, port, proxyType, server));
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

    QFile playlistFile(fileName);
    if (!playlistFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    ui->rangesText->clear();
    ui->countryText->clear();
    ui->networkText->clear();
    ui->portsText->clear();
    tableModel->reset();

    static QRegularExpression newlineRegex("[\r\n]+");
    QStringList lines = QString(playlistFile.readAll()).split(newlineRegex);
    ParseResult parseResult = playlistParser.parsePlaylist(lines);

    IpInfo ipInfo = whoisClient->getIpInfo(parseResult.ip(), parseResult.port());

    ui->rangesText->appendPlainText(ipInfo.mainRange());
    for (const QString& range : ipInfo.otherRanges())
        ui->rangesText->appendPlainText(range);

    ui->countryText->setText(ipInfo.country());
    ui->networkText->setText(ipInfo.networkName());
    ui->portsText->appendPlainText(QString::number(parseResult.port()));

    maskedLine = parseResult.maskedLine();
    maskedPlaylist = parseResult.maskedPlaylist();

    ui->searchButton->setEnabled(true);
}

QString MainWindow::replaceIpAndPort(QString str, const QString& newIp, ushort newPort)
{
    return str
        .replace(playlistParser.ipPlaceholder(), newIp)
        .replace(playlistParser.portPlaceholder(), QString::number(newPort));
}

void MainWindow::searchCompleted()
{
    lastIpPort.clear();
    ui->pauseButton->setEnabled(false);
    ui->stopButton->setEnabled(false);
    ui->connectSpin->setEnabled(true);
    ui->countryText->setEnabled(true);
    ui->networkText->setEnabled(true);
    ui->playlistButton->setEnabled(true);
    ui->playlistText->setEnabled(true);
    ui->portsText->setEnabled(true);
    ui->rangesText->setEnabled(true);
    ui->searchButton->setEnabled(true);
}

void MainWindow::startSearch()
{
    if (ui->rangesText->document()->isEmpty() || ui->portsText->document()->isEmpty())
        return;

    tableModel->reset();

    ui->pauseButton->setEnabled(true);
    ui->saveButton->setEnabled(true);
    ui->stopButton->setEnabled(true);
    ui->connectSpin->setEnabled(false);
    ui->countryText->setEnabled(false);
    ui->networkText->setEnabled(false);
    ui->playlistButton->setEnabled(false);
    ui->playlistText->setEnabled(false);
    ui->portsText->setEnabled(false);
    ui->rangesText->setEnabled(false);
    ui->searchButton->setEnabled(false);

    QtConcurrent::run(
        &Scanner::findProxies, scanner,
        ui->rangesText->toPlainText(),
        ui->portsText->toPlainText(),
        ui->connectSpin->value(),
        [this](const QString& ip, ushort port) { return replaceIpAndPort(maskedLine, ip, port); }
    ).then(std::bind(&MainWindow::searchCompleted, this));
}

void MainWindow::updateLastIpPort(const QString& ip, ushort port)
{
    lastIpPort = ip + ":" + port;
}

void MainWindow::updateProgress(int completions, int total)
{
    ui->progressBar->setMaximum(total);
    ui->progressBar->setValue(completions);
}
