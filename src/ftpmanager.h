#ifndef FTPMANAGER_H
#define FTPMANAGER_H

#include <QObject>
#include <QFtp>
#include <QUrl>

class FtpManager : public QObject
{
    Q_OBJECT

public:
    FtpManager(const QString& url, QObject *parent = nullptr);

    FtpManager(QObject *parent = nullptr);

    void connectToHost(const QString& url);

    void list(const QString& dir = QString());

    const std::vector<QUrlInfo>& getLastDirList() const;

    QFtp* getClient() const;

signals:
    void connected();

    void listDone(const std::vector<QUrlInfo> &dirList);

    void downloaded(const QByteArray &data);

    // TODO parameter
    void error();

private slots:
    void procResult(int id, bool err);

    void addListItem(const QUrlInfo& item);

    void commandStart(int id);

private:

    QFtp *_client = nullptr;

    QString _url;

    std::vector<QUrlInfo> _dirList;
};

#endif // FTPMANAGER_H
