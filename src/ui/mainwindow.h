#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QUrl>

#include "ftpadapter.h"

class QNetworkAccessManager;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void sendRequest();

    void error();

    void listDirectory(std::vector<QUrlInfo> info);

    void connected();

private:
    QTextEdit *_receivedData = nullptr;
    FtpAdapter *_ftp = nullptr;
//    QNetworkAccessManager *_networkManager = nullptr;
};
#endif // MAINWINDOW_H
