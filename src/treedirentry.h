#ifndef TREEDIRENTRY_H
#define TREEDIRENTRY_H

#include <QList>
#include <QDate>
#include <QVariant>
#include <QString>

#include "qurlinfo.h"

class TreeDirEntry : public QUrlInfo
{
public:
    TreeDirEntry(QString name)
        : QUrlInfo()
    {
        setName(name);
    }

    TreeDirEntry(const QUrlInfo& info, TreeDirEntry *parent)
        : QUrlInfo(info), _parent(parent)
    {}

    ~TreeDirEntry()
    {
        qDeleteAll(_children);
    }

    void addChild(TreeDirEntry *child)
    {
        child->_parent = this;
        _children.append(child);
    }

    TreeDirEntry* getParent() const
    {
        return _parent;
    }

    TreeDirEntry* getChild(int id) const
    {
        return _children.at(id);
    }

    int countChild() const
    {
        return _children.size();
    }

    int row() const
    {
        if (!_parent)
            return 0;

        return _parent->_children.indexOf(const_cast<TreeDirEntry*>(this));
    }

    QVariant data(int column) const
    {
        switch (column) {
        case NAME:
            return name();

        case MODIFY_DATE:
            return lastModified();

        case FILE_SIZE:
            return size();

        default:
            return QVariant();
        }
    }

    static int columnCount()
    {
        return COLUMN_SIZE;
    }

    static QString columnNameAt(int column)
    {
        static const std::map<int, QString> namePerColumn {
            {NAME, "Name"},
            {MODIFY_DATE, "Modify date"},
            {FILE_SIZE, "Size"},
        };
        return namePerColumn.count(column) ? namePerColumn.at(column) : "";
    }


private:
    enum Column {
        NAME,
        FILE_SIZE,
        MODIFY_DATE,
        COLUMN_SIZE
    };

    TreeDirEntry *_parent = nullptr;
    QList<TreeDirEntry*> _children;
};

#endif // TREEDIRENTRY_H
