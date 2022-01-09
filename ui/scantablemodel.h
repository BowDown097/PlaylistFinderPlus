#ifndef SCANTABLEMODEL_H
#define SCANTABLEMODEL_H
#include <ui/scanresult.h>
#include <QAbstractTableModel>
#include <QList>

class ScanTableModel : public QAbstractTableModel
{
    QList<ScanResult> m_data;
public:
    ScanTableModel(QObject* parent = {}) : QAbstractTableModel{parent} {}

    int rowCount(const QModelIndex&) const override { return m_data.count(); }
    int columnCount(const QModelIndex&) const override { return 4; }

    QVariant data(const QModelIndex& index, int role) const override
    {
        if (role != Qt::DisplayRole && role != Qt::EditRole) return {};
        const ScanResult& scanResult = m_data[index.row()];
        switch (index.column())
        {
            case 0: return scanResult.sel ? "Y" : "N";
            case 1: return scanResult.ip;
            case 2: return scanResult.port;
            case 3: return scanResult.info;
            default: return {};
        }
    }

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override
    {
        if (orientation != Qt::Horizontal || role != Qt::DisplayRole) return {};
        switch (section)
        {
            case 0: return "Sel";
            case 1: return "IP";
            case 2: return "Port";
            case 3: return "Info";
            default: return {};
        }
    }

    void append(const ScanResult& scanResult)
    {
        beginInsertRows({}, m_data.count(), m_data.count());
        m_data.append(scanResult);
        endInsertRows();
    }
};

#endif // SCANTABLEMODEL_H
