#include <QtDebug>

#include "ftpmanager.h"

FtpManager::FtpManager(QObject *parent) : QObject(parent)
{
    _client = new QFtp(this);

    QObject::connect(_client, &QFtp::commandFinished,
                     this, &FtpManager::procResult);

    QObject::connect(_client, &QFtp::commandStarted,
                     this, &FtpManager::commandStart);

    QObject::connect(_client, &QFtp::listInfo, this, &FtpManager::addListItem);
}

FtpManager::FtpManager(const QString& url, QObject *parent)
    : FtpManager(parent)
{
    _url = url;
    _client->connectToHost(_url);
    _dirList.reserve(30);
}

void FtpManager::procResult(int id, bool err)
{
    if (err) {
        emit error();
        return;
    }

    switch (_client->currentCommand()) {
    case QFtp::ConnectToHost:
        qDebug() << "connected to host" << Qt::endl;
        emit connected();
        break;

    case QFtp::List:
        qDebug() << "get list" << Qt::endl;
        emit listDone(_dirList);
        break;

    default:
        break;
    }

}



void FtpManager::connectToHost(const QString &url)
{
    if (_client->hasPendingCommands()) {
        _client->clearPendingCommands();
        _client->abort();
    }

    _url = url;
    _client->connectToHost(_url);
}

void FtpManager::list(const QString &dir)
{
    _client->list(dir);
}

QFtp* FtpManager::getClient() const
{
    return _client;
}
