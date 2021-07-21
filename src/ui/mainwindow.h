#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUrl>

class QFtp;
class QUrl;
class QUrlInfo;
class QTextEdit;

class QNetworkAccessManager;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void sendRequest();

    void errorOccured();

    void addListEntry(const QUrlInfo& entry);

    void ftpConnect(const QUrl &url = QUrl());

    void ftpCommandFinished(int id, bool error);

private:
    void constructWidgetLayout();

    QTextEdit *_receivedData = nullptr;
    QFtp *_ftp = nullptr;
};
#endif // MAINWINDOW_H
