#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUrl>

class QFtp;
class QUrl;
class QUrlInfo;
class QTextEdit;
class QTreeWidget;
class QGroupBox;
class QTreeView;

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

    void ftpCommandStarted(int id);

    void ftpCommandFinished(int id, bool error);

private:
    void constructWidgetLayout();

    QGroupBox* createConnectionGroup();

    QTreeView* createTreeDirView();

    QTreeView *_dir;

    QFtp *_ftp = nullptr;
};
#endif // MAINWINDOW_H
