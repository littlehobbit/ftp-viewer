#include <iostream>

#include <QPushButton>
#include <QBoxLayout>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QMenuBar>
#include <QToolBar>
#include <QGroupBox>
#include <QTreeWidget>
#include <QMessageBox>

#include <QFtp>
#include <QUrlInfo>
#include <QTextEdit>

#include "mainwindow.h"
#include "urlloginwidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->resize(500, 500);

    // TODO: refactor this func
    constructWidgetLayout();
}

void MainWindow::constructWidgetLayout()
{
    // Application frame
    QFrame *frame = new QFrame();
    this->setCentralWidget(frame);

    // Button configuration
    QPushButton *sendButton = new QPushButton("Send request");
    QObject::connect(
            sendButton, &QPushButton::pressed,
            this, &MainWindow::sendRequest
    );

    // Layout
    auto urlLoginWidget = new UrlLoginWidget;
    auto urlLoginLayout = new QVBoxLayout;
    urlLoginLayout->addWidget(urlLoginWidget);

    QObject::connect(urlLoginWidget, &UrlLoginWidget::connectRequest,
                     this, &MainWindow::ftpConnect);

    auto urlLoginGroup = new QGroupBox(tr("Connection"));
    urlLoginGroup->setLayout(urlLoginLayout);

    QBoxLayout *rightLayout = new QVBoxLayout();
    rightLayout->addWidget(urlLoginGroup);
    rightLayout->addStretch(1);
    rightLayout->addWidget(sendButton);
    rightLayout->setSizeConstraint(QLayout::SetFixedSize);

    auto widgetLayout = new QHBoxLayout();


    _dir = createTreeDirView();
    widgetLayout->addWidget(_dir);
    widgetLayout->addLayout(rightLayout);

    frame->setLayout(widgetLayout);
}

QTreeWidget* MainWindow::createTreeDirView()
{
    const QStringList headerLabels = {"Name", "Size", "Owner"};

    auto treeWidget = new QTreeWidget();
    treeWidget->setHeaderLabels(headerLabels);
//    QList<QTreeWidgetItem *> items;
//    for (int i = 0; i < 10; ++i)
//        items.append(new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr), QStringList({"node", "2", "me"})));
//    treeWidget->insertTopLevelItems(0, items);

    return treeWidget;
}

void MainWindow::sendRequest()
{
    _ftp->list();
}

void MainWindow::errorOccured()
{
    QMessageBox::critical(
      this,
      tr("Erorr"),
      _ftp->errorString()
    );
}

void MainWindow::addListEntry(const QUrlInfo& entry)
{
    QStringList newEntry;
    newEntry.append(entry.name());
    newEntry.append(QString::number(entry.size()));
    newEntry.append(entry.owner());

    _dir->addTopLevelItem(new QTreeWidgetItem(newEntry));
}

void MainWindow::ftpConnect(const QUrl &url)
{
    // Debug info
    QTextStream(stdout) << url.host() << ' ' << url.userName() << ' ' << url.password() << ' ' << Qt::endl;

    if (_ftp) {
        // Abort last command and delete connection
        _ftp->abort();
        _ftp->deleteLater();
    }

    // New ftp connection
    _ftp = new QFtp(this);
    _ftp->connectToHost(url.host(), url.port(21));

    QString userName = url.userName();
    if (userName.isEmpty()) {
        _ftp->login("anonymous", url.password());
    } else {
        _ftp->login(userName, url.password());
    }

    // ftp configure
    QObject::connect(_ftp, &QFtp::listInfo,
                     this, &MainWindow::addListEntry);

    QObject::connect(_ftp, &QFtp::commandStarted,
                     this, &MainWindow::ftpCommandStarted);

    QObject::connect(_ftp, &QFtp::commandFinished,
                     this, &MainWindow::ftpCommandFinished);

    QObject::connect(_ftp, &QFtp::stateChanged,
                     this, [](int state) {
        std::cout << "state: ";
        switch (state) {
        case QFtp::Unconnected:
            std::cout << "Unconnected";
            break;
        case QFtp::HostLookup:
            std::cout << "HostLookup";
            break;
        case QFtp::Connecting:
            std::cout << "Connecting";
            break;
        case QFtp::Connected:
            std::cout << "Connected";
            break;
        case QFtp::LoggedIn:
            std::cout << "LoggedIn";
            break;
        case QFtp::Closing:
            std::cout << "Closing";
            break;
        }
        std::cout << std::endl;
    });

}

void MainWindow::ftpCommandStarted(int id)
{
    switch (_ftp->currentCommand()) {
    case QFtp::List:
        _dir->clear();
        break;
    default:
        break;
    }
}

void MainWindow::ftpCommandFinished(int id, bool error)
{
    if (error) {
        errorOccured();
        return;
    }

    // Result proccessing
    switch (_ftp->currentCommand()) {
    default:
        break;
    }
}

MainWindow::~MainWindow()
{
}

