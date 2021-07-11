#ifndef FTPMANAGER_H
#define FTPMANAGER_H

#include <QObject>
#include <QFtp>
#include <QUrl>

class FtpAdapter : public QFtp
{
    Q_OBJECT

public:
    FtpAdapter(const QString& url, QObject *parent = nullptr);

    FtpAdapter(QObject *parent = nullptr);

    void connectToHost(const QString& url);

    const std::vector<QUrlInfo>& getLastDirList() const;

signals:
    void connected();

    void listDone(const std::vector<QUrlInfo> dirList);

    void downloaded(const QByteArray &data);

    void errorOccured();

private slots:
    void procResult(int id, bool err);

    void addListItem(const QUrlInfo& item);

    void commandStart(int id);

private:
    std::vector<QUrlInfo> _dirList;

};

#endif // FTPMANAGER_H
