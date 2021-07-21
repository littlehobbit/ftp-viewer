#include <iostream>

#include <QPushButton>
#include <QBoxLayout>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QMenuBar>
#include <QToolBar>
#include <QGroupBox>

#include <QFtp>
#include <QUrlInfo>
#include <QTextEdit>

#include "mainwindow.h"
#include "urlloginwidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->resize(500, 500);


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

    // Text box
    _receivedData = new QTextEdit("Here will be the answner", this);
    _receivedData->setReadOnly(true);

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

    auto widgetLayout = new QHBoxLayout();
    widgetLayout->addWidget(_receivedData);
    widgetLayout->addLayout(rightLayout);

    frame->setLayout(widgetLayout);
}

void MainWindow::sendRequest()
{
}

void MainWindow::errorOccured()
{
    QTextStream out(stdout);
    out << _ftp->errorString() << Qt::endl;
}

void MainWindow::addListEntry(const QUrlInfo& entry)
{
    QString text = _receivedData->toPlainText();
    text += '\n' + entry.name();
}

void MainWindow::ftpConnect(const QUrl &url)
{
    QTextStream(stdout) << url.host() << ' ' << url.userName() << ' ' << url.password() << Qt::endl;

    if (_ftp) {
        _ftp->abort();
        _ftp->deleteLater();
    }

    _ftp = new QFtp(this);
    _ftp->connectToHost(url.host(), url.port(21));

    QObject::connect(_ftp, &QFtp::listInfo,
                     this, &MainWindow::addListEntry);

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

void MainWindow::ftpCommandFinished(int id, bool error)
{
    if (error) {
        errorOccured();
        return;
    }

    switch (_ftp->currentCommand()) {
    default:
        break;
    }
}

MainWindow::~MainWindow()
{
}

