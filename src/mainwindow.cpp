#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "playlistparser/playlistparser.h"
#include <QFileDialog>
#include <QThreadPool>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      scanner(new Scanner(this)),
      ui(new Ui::MainWindow),
      whoisClient(new WhoisClient(this))
{
    ui->setupUi(this);
    // create table
    tableModel = new ScanTableModel(ui->tableView);
    ui->tableView->setModel(tableModel);
    ui->tableView->setColumnWidth(0, 25);
    // set up buttons
    connect(ui->pauseButton, &QPushButton::clicked, this, &MainWindow::togglePauseSearch);
    connect(ui->playlistButton, &QPushButton::clicked, this, &MainWindow::loadPlaylist);
    connect(ui->saveButton, &QPushButton::clicked, this, &MainWindow::multipleSavePlaylist);
    connect(ui->searchButton, &QPushButton::clicked, this, &MainWindow::startSearch);
    connect(ui->stopButton, &QPushButton::clicked, this, &MainWindow::stopSearch);
    // set up scanner
    connect(scanner, &Scanner::lastIpPort, this, &MainWindow::updateLastIpPort);
    connect(scanner, &Scanner::newProxyFound, this, &MainWindow::addProxy);
    connect(scanner, &Scanner::progress, this, &MainWindow::updateProgress);
}

MainWindow::~MainWindow()
{
    delete ui;
    ui = nullptr;
    scanner->stop();
}

void MainWindow::addProxy(const QString& ip, ushort port, const QString& proxyType, const QString& server)
{
    ui->saveButton->setEnabled(true);
    tableModel->append(ScanResult(ip, port, proxyType, server));
}

void MainWindow::loadPlaylist()
{
    playlistPath = QFileDialog::getOpenFileName(
        this,
        "Choose playlist file",
        QString(),
        "m3u (*.m3u *.m3u8);;All files (*.*)"
    );

    ui->playlistText->setText(playlistPath);

    QFile playlistFile(playlistPath);
    if (!playlistFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    ui->rangesText->clear();
    ui->countryText->clear();
    ui->networkText->clear();
    ui->portsText->clear();
    tableModel->reset();

    static QRegularExpression newlineRegex("[\r\n]+");
    QStringList lines = QString(playlistFile.readAll()).split(newlineRegex);
    ParseResult parseResult = PlaylistParser::parsePlaylist(lines);

    IpInfo ipInfo = whoisClient->getIpInfo(parseResult.ip(), parseResult.port());

    ui->rangesText->appendPlainText(ipInfo.mainRange());
    for (const QString& range : ipInfo.otherRanges())
        ui->rangesText->appendPlainText(range);

    ui->countryText->setText(ipInfo.country());
    ui->networkText->setText(ipInfo.networkName());
    ui->portsText->setPlainText(QString::number(parseResult.port()));

    ui->networkText->setCursorPosition(0);
    ui->rangesText->moveCursor(QTextCursor::Start);

    maskedLine = parseResult.maskedLine();
    maskedPlaylist = parseResult.maskedPlaylist();

    ui->searchButton->setEnabled(true);
}

void MainWindow::multipleSavePlaylist()
{
    int rowCount = tableModel->rowCount(QModelIndex());
    if (rowCount == 0)
        return;

    QFileInfo playlistInfo(playlistPath);
    QString ext = playlistInfo.completeSuffix();

    QString selectedPath = QFileDialog::getExistingDirectory(
        this,
        QString(),
        playlistInfo.dir().absolutePath(),
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
    );

    QDir selectedDir(selectedPath);
    if (!selectedDir.exists())
        return;

    bool saveSelected = ui->selectedIPs->isChecked();
    ui->saveButton->setEnabled(false);

    QStringList lines;
    lines.append("#EXTM3U");
    int playlistNum = 0;

    for (int i = 0; i < rowCount; i++)
    {
        const ScanResult& result = tableModel->result(i);
        if (saveSelected && !result.sel)
            continue;

        playlistNum++;
        QString outNum = QString::number(playlistNum).rightJustified(4, '0');

        QFile outFile(selectedDir.filePath(outNum + "." + ext));
        if (!outFile.open(QIODevice::WriteOnly))
            continue;

        QTextStream outStream(&outFile);
        outStream << replaceIpAndPort(maskedPlaylist, result.ip, result.port);

        QString host = result.ip + ":" + QString::number(result.port);
        lines.append(QStringLiteral("#EXTINF:-1,%1 (%2)").arg(outNum, host));
        lines.append(replaceIpAndPort(maskedLine, result.ip, result.port));
    }

    QFile allFile(selectedDir.filePath("_all_." + ext));
    if (!allFile.open(QIODevice::WriteOnly))
        return;

    QTextStream allStream(&allFile);
    for (const QString& line : lines)
        allStream << line << Qt::endl;

    ui->saveButton->setEnabled(true);
}

QString MainWindow::replaceIpAndPort(QString str, const QString& newIp, ushort newPort)
{
    return str
        .replace(PlaylistParser::ipMask, newIp)
        .replace(PlaylistParser::portMask, QString::number(newPort));
}

void MainWindow::searchCompleted()
{
    if (!ui)
        return;

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
    ui->stopButton->setEnabled(true);
    ui->connectSpin->setEnabled(false);
    ui->countryText->setEnabled(false);
    ui->networkText->setEnabled(false);
    ui->playlistButton->setEnabled(false);
    ui->playlistText->setEnabled(false);
    ui->portsText->setEnabled(false);
    ui->rangesText->setEnabled(false);
    ui->saveButton->setEnabled(false);
    ui->searchButton->setEnabled(false);

    QRunnable* runnable = QRunnable::create([this] {
        scanner->findProxies(
            ui->rangesText->toPlainText(),
            ui->portsText->toPlainText(),
            ui->connectSpin->value(),
            [this](const QString& ip, ushort port) { return replaceIpAndPort(maskedLine, ip, port); }
        );
        searchCompleted();
    });

    QThreadPool::globalInstance()->start(runnable);
}

void MainWindow::stopSearch()
{
    scanner->stop();
    ui->stopButton->setEnabled(false);
}

void MainWindow::togglePauseSearch()
{
    scanner->pause();
    ui->pauseButton->setText(scanner->paused() ? "Resume" : "Pause");
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
