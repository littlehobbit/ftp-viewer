#include <QAbstractItemModel>

#include "ftpdirmodel.h"

FtpDirModel::FtpDirModel(QObject *parent)
    : QAbstractItemModel(parent), _root(new TreeDirEntry("/"))
{}

FtpDirModel::~FtpDirModel()
{
    delete _root;
}

QVariant FtpDirModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    return makeHeader(section, orientation);
}

QModelIndex FtpDirModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TreeDirEntry *parentDir = getTreeEntryPtrFrom(parent);
    if (!parentDir)
        return createIndex(row, column, _root);

    TreeDirEntry *childDir = parentDir->getChild(row);
    if (childDir)
        return createIndex(row, column, childDir);

    return QModelIndex();
}

QModelIndex FtpDirModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeDirEntry *child = getTreeEntryPtrFrom(index);
    TreeDirEntry *parent = child->getParent();


    if (!parent)
        return QModelIndex();

    return createIndex(parent->row(), 0, parent);
}

QVariant FtpDirModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole)
        return QVariant();

    TreeDirEntry *entry = getTreeEntryPtrFrom(index);
    return entry->data(index.column());
}

int FtpDirModel::rowCount(const QModelIndex &parent) const
{
    TreeDirEntry *parentDir = getTreeEntryPtrFrom(parent);
    if (!parentDir)
        return 1;

    return parentDir->countChild();
}

int FtpDirModel::columnCount(const QModelIndex&) const
{
    return TreeDirEntry::columnCount();
}

QVariant FtpDirModel::makeHeader(int column, Qt::Orientation orientation)
{
    if (orientation == Qt::Vertical)
        return QVariant();

    return TreeDirEntry::columnNameAt(column);
}

TreeDirEntry* FtpDirModel::getTreeEntryPtrFrom(const QModelIndex &index) const
{
    return static_cast<TreeDirEntry*>(index.internalPointer());
}

void FtpDirModel::deleteEntries()
{
    beginRemoveRows(QModelIndex(), 0, 1);
    _root->deleteChildren();
    endRemoveRows();
}

void FtpDirModel::setRoot(const QString &rootName, const QList<QUrlInfo> &entries)
{
    deleteEntries();

    _root->setName(rootName);
    addSubDirectoryEntries(entries);
}

void FtpDirModel::addSubDirectoryEntries(const QList<QUrlInfo> &entries, const QModelIndex &parent)
{
    beginInsertRows(parent, 0, entries.size());

    TreeDirEntry *parentDir = getTreeEntryPtrFrom(parent);
    if (!parentDir)
        parentDir = _root;

    for (auto entry : entries)
        parentDir->addChild(new TreeDirEntry(entry));

    endInsertRows();
}
