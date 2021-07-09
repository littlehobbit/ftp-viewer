#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QUrl>

#include <QFtp>

#include "ftpmanager.h"

class QNetworkAccessManager;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void sendRequest();

private:
    QTextEdit *_receivedData = nullptr;
    QFtp *_ftp = nullptr;
    QNetworkAccessManager *_networkManager = nullptr;
};
#endif // MAINWINDOW_H
