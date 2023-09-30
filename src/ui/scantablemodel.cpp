#include "scantablemodel.h"

void ScanTableModel::append(const ScanResult& scanResult)
{
    beginInsertRows({}, m_data.count(), m_data.count());
    m_data.append(scanResult);
    endInsertRows();
}

QVariant ScanTableModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    const ScanResult& scanResult = m_data.at(index.row());
    switch (role)
    {
    case Qt::DisplayRole:
        switch (index.column())
        {
        case 1: return scanResult.ip;
        case 2: return scanResult.port;
        case 3: return scanResult.info;
        default: return QVariant();
        }
    case Qt::CheckStateRole:
        if (index.column() == 0)
            return scanResult.sel ? Qt::Checked : Qt::Unchecked;
        return QVariant();
    default:
        return QVariant();
    }
}

Qt::ItemFlags ScanTableModel::flags(const QModelIndex& index) const
{
    if (!index.isValid())
        return QAbstractTableModel::flags(index);

    Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    if (index.column() == 0)
        flags |= Qt::ItemIsUserCheckable;

    return flags;
}

QVariant ScanTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
        return QVariant();

    switch (section)
    {
    case 0: return "Sel";
    case 1: return "IP";
    case 2: return "Port";
    case 3: return "Info";
    default: return QVariant();
    }
}

void ScanTableModel::reset()
{
    beginResetModel();
    m_data.clear();
    endResetModel();
}

bool ScanTableModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (!index.isValid())
        return false;

    ScanResult scanResult = m_data[index.row()];
    switch (role)
    {
    case Qt::DisplayRole:
        switch (index.column())
        {
        case 1: scanResult.ip = value.toString(); break;
        case 2: scanResult.port = value.toInt(); break;
        case 3: scanResult.info = value.toString(); break;
        default: return false;
        }

        m_data.replace(index.row(), scanResult);
        emit dataChanged(index, index);
        return true;
    case Qt::CheckStateRole:
        if (index.column() == 0)
        {
            scanResult.sel = (value.toInt() == Qt::Checked);
            m_data.replace(index.row(), scanResult);
            emit dataChanged(index, index);
            return true;
        }
        return false;
    default:
        return false;
    }
}
