#ifndef SCANTABLEMODEL_H
#define SCANTABLEMODEL_H
#include "scanresult.h"
#include <QAbstractTableModel>
#include <QList>

class ScanTableModel : public QAbstractTableModel
{
public:
    explicit ScanTableModel(QObject* parent = nullptr) : QAbstractTableModel(parent) {}

    int columnCount(const QModelIndex&) const override { return 4; }
    int rowCount(const QModelIndex&) const override { return m_data.count(); }

    void append(const ScanResult& scanResult);
    QVariant data(const QModelIndex& index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role) override;
private:
    QList<ScanResult> m_data;
};

#endif // SCANTABLEMODEL_H
