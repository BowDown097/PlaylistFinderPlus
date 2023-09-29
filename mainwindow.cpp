#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui/scantablemodel.h"
#include <QFileDialog>

// TODO:
// MainWindow: All button stuff (loading, searching, etc.)
// Scanner: httpheaders.h, scanner.h

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // create table
    ScanTableModel* tableModel = new ScanTableModel(ui->tableView);
    ui->tableView->setModel(tableModel);
    ui->tableView->setColumnWidth(0, 25);
    // set up buttons
    connect(ui->playlistButton, SIGNAL(clicked()), this, SLOT(loadPlaylist()));
    connect(ui->searchButton, SIGNAL(clicked()), this, SLOT(startSearch()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadPlaylist()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Choose playlist file"), QString(), tr("m3u (*.m3u *.m3u8);;All files (*.*"));
    ui->playlistText->setText(fileName);
    ui->searchButton->setEnabled(true);
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
