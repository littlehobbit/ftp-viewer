#include <QDebug>

#include "ftpadapter.h"

FtpAdapter::FtpAdapter(QObject *parent) : QFtp(parent)
{
    QObject::connect(this, &QFtp::commandFinished,
                     this, &FtpAdapter::procResult);

    QObject::connect(this, &QFtp::commandStarted,
                     this, &FtpAdapter::commandStart);

    QObject::connect(this, &QFtp::listInfo, this, &FtpAdapter::addListItem);
}

FtpAdapter::FtpAdapter(const QString& url, QObject *parent)
    : QFtp(parent)
{
    this->connectToHost(url);
    _dirList.reserve(30);
}

void FtpAdapter::procResult(int id, bool err)
{
    if (err) {
        emit errorOccured();
        return;
    }

    switch (this->currentCommand()) {
    case QFtp::ConnectToHost:
        QTextStream(stdout) << "connected to host" << Qt::endl;
        emit connected();
        break;

    case QFtp::List:
        QTextStream(stdout) << "get list" << Qt::endl;
        emit listDone(_dirList);
        break;

    default:
        break;
    }
}

void FtpAdapter::commandStart(int id)
{
    switch (this->currentCommand()) {
    case QFtp::List:
        _dirList.clear();
        break;

    default:
        break;
    }
}

void FtpAdapter::connectToHost(const QString &url)
{
    if (this->hasPendingCommands()) {
        this->clearPendingCommands();
        this->abort();
    }

//    _url = url;
    QFtp::connectToHost(url);
}

void FtpAdapter::addListItem(const QUrlInfo& item)
{
    _dirList.push_back(item);
}

//void FtpManager::list(const QString &dir)
//{
//    _client->list(dir);
//}

//QFtp* FtpManager::getClient() const
//{
//    return _client;
//}
