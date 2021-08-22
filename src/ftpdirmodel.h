#ifndef FTPDIRMODEL_H
#define FTPDIRMODEL_H

#include <QAbstractItemModel>

#include "treedirentry.h"

class FtpDirModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    FtpDirModel(QObject *parent = nullptr);

    ~FtpDirModel();

    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    QModelIndex index(int row,
                      int column,
                      const QModelIndex &parent = QModelIndex()) const override;

    QModelIndex parent(const QModelIndex &index) const override;

    QVariant data(const QModelIndex &index,
                  int role = Qt::DisplayRole) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    void addSubDirectoryEntries(const QList<QUrlInfo> &entries,
                    const QModelIndex &parent = QModelIndex());

    void deleteEntries();

    void setRoot(const QString &rootName, const QList<QUrlInfo> &entries);

private:
    static QVariant makeHeader(int column, Qt::Orientation orientation);

    TreeDirEntry* getTreeEntryPtrFrom(const QModelIndex &index) const;

    TreeDirEntry *_root = nullptr;
};

#endif // FTPDIRMODEL_H
