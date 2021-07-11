#include <QDebug>

#include "ftpadapter.h"

class FtpAdapter::FtpAdapterDestroyer
{
public:
    FtpAdapterDestroyer() = default;

    ~FtpAdapterDestroyer()
    {
        delete _instance;
    }

    void initiaize(FtpAdapter* ptr)
    {
        _instance = ptr;
    }

private:
    FtpAdapter *_instance = nullptr;

};

FtpAdapter* FtpAdapter::_instance = nullptr;
FtpAdapter::FtpAdapterDestroyer FtpAdapter::_destoyer;

FtpAdapter::FtpAdapter(QObject *parent) : QFtp(parent)
{
    QObject::connect(this, &QFtp::commandFinished,
                     this, &FtpAdapter::procResult);

    QObject::connect(this, &QFtp::commandStarted,
                     this, &FtpAdapter::commandStart);

    QObject::connect(this, &QFtp::listInfo, this, &FtpAdapter::addListItem);

    _dirList.reserve(30);
}

FtpAdapter::FtpAdapter(const QString& url, QObject *parent)
    : QFtp(parent)
{
    this->connectToHost(url);
}

FtpAdapter& FtpAdapter::instance()
{
    if (!_instance) {
        _instance = new FtpAdapter();
        _destoyer.initiaize(_instance);
    }

    return *_instance;
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
    if (instance().hasPendingCommands()) {
        instance().clearPendingCommands();
        instance().abort();
    }

    instance().QFtp::connectToHost(url);
}

void FtpAdapter::addListItem(const QUrlInfo& item)
{
    _dirList.push_back(item);
}
