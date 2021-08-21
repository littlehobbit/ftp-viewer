#ifndef FTPDIRMODEL_H
#define FTPDIRMODEL_H

#include <QAbstractItemModel>
#include <QDateTime>
#include <QDebug>

#include "treedirentry.h"

class FtpDirModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    FtpDirModel(QObject *parent = nullptr)
        : QAbstractItemModel(parent)
    {
        _root = new TreeDirEntry(QUrlInfo(), nullptr);
        _root->setName("Root dir");

        TreeDirEntry *en = new TreeDirEntry("sub dir");
        _root->addChild(en);

        _root->addChild(new TreeDirEntry("sub sub dir"));
        _root->getChild(0)->addChild(new TreeDirEntry("sub ssss dir"));
    }

    ~FtpDirModel()
    {
        delete _root;
    }

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override
    {
        if (role != Qt::DisplayRole)
            return QVariant();

        return makeHeader(section, orientation);
    }

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override
    {
        if (!hasIndex(row, column, parent))
            return QModelIndex();

        TreeDirEntry *parentDir;
        if (!parent.isValid())
            return createIndex(row, column, _root);
        else
            parentDir = static_cast<TreeDirEntry*>(parent.internalPointer());

        TreeDirEntry *childDir = parentDir->getChild(row);
        if (childDir)
            return createIndex(row, column, childDir);

        return QModelIndex();
    }

    QModelIndex parent(const QModelIndex &index) const override
    {
        if (!index.isValid())
            return QModelIndex();

        TreeDirEntry *child = static_cast<TreeDirEntry*>(index.internalPointer());
        TreeDirEntry *parent = child->getParent();

        if (child == parent || !parent)
            return QModelIndex();

        return createIndex(parent->row(), 0, parent);
    }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override
    {
        if (!index.isValid() || role != Qt::DisplayRole)
            return QVariant();

        TreeDirEntry *entry = static_cast<TreeDirEntry*>(index.internalPointer());
        return entry->data(index.column());
    }

    int rowCount(const QModelIndex &parent = QModelIndex()) const override
    {
//        if (parent.column() > 0)
//            return 0;

        TreeDirEntry *parentDir;
        if (!parent.isValid())
            parentDir = _root;
        else
            parentDir = static_cast<TreeDirEntry*>(parent.internalPointer());

        return parentDir->countChild();
    }

    int columnCount(const QModelIndex &parent = QModelIndex()) const override
    {
        return TreeDirEntry::columnCount();
    }


private:
    static QVariant makeHeader(int column, Qt::Orientation orientation)
    {
        if (orientation == Qt::Vertical)
            return QVariant();

        return TreeDirEntry::columnNameAt(column);
    }

    TreeDirEntry *_root = nullptr;
};

#endif // FTPDIRMODEL_H
