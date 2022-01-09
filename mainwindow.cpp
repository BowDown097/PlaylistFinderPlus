#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <ui/scantablemodel.h>

// TODO:
// Scanner: httpheaders.h, scanner.h
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->rangesText->setPlainText("- TEST IPS -\n37.192.0.0 - 37.195.255.255\n5.128.0.0 - 5.131.255.255\n80.64.86.0 - 80.64.87.255\n176.53.128.0 - 176.53.191.255\n178.49.0.0 - 178.49.255.255");
    ui->countryText->setText("US");
    ui->ddnsDrop->addItem("Troll HQ");
    ui->networkText->setText("BLACKED: Exclusive Big Dick HD Erotica Porn Videos");
    ui->portsText->setPlainText("80\n81\n8080\n25\n23\n8081");

    ScanTableModel* tableModel = new ScanTableModel(ui->tableView);
    tableModel->append(ScanResult(true, "192.168.1.1", "test", 255));
    tableModel->append(ScanResult(true, "192.168.1.1", "test", 255));
    tableModel->append(ScanResult(true, "192.168.1.1", "test", 255));
    tableModel->append(ScanResult(true, "192.168.1.1", "test", 255));
    tableModel->append(ScanResult(true, "192.168.1.1", "test", 255));
    tableModel->append(ScanResult(true, "192.168.1.1", "test", 255));
    tableModel->append(ScanResult(true, "192.168.1.1", "test", 255));
    tableModel->append(ScanResult(true, "192.168.1.1", "test", 255));
    tableModel->append(ScanResult(true, "192.168.1.1", "test", 255));
    tableModel->append(ScanResult(true, "192.168.1.1", "test", 255));
    tableModel->append(ScanResult(true, "192.168.1.1", "test", 255));
    tableModel->append(ScanResult(true, "192.168.1.1", "test", 255));
    tableModel->append(ScanResult(true, "192.168.1.1", "test", 255));
    tableModel->append(ScanResult(true, "192.168.1.1", "test", 255));
    tableModel->append(ScanResult(true, "192.168.1.1", "test", 255));
    tableModel->append(ScanResult(true, "192.168.1.1", "test", 255));
    ui->tableView->setModel(tableModel);
}

MainWindow::~MainWindow()
{
    delete ui;
}

