#include <iostream>

#include <QPushButton>
#include <QBoxLayout>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QMenuBar>
#include <QToolBar>

#include "mainwindow.h"
#include "urlloginwidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->resize(500, 500);

    // Temp signal checking
    auto& ftp = FtpAdapter::instance();
    QObject::connect(&ftp, &FtpAdapter::errorOccured,
                     this, &MainWindow::error);

    QObject::connect(&ftp, &FtpAdapter::listDone,
                     this, &MainWindow::listDirectory);


    ftp.connectToHost("ftp.gnu.org");
    ftp.login();

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
    auto urlLoginForm = new UrlLoginWidget();
    QBoxLayout *rightLayout = new QVBoxLayout();
    rightLayout->addWidget(urlLoginForm);
    rightLayout->addStretch(1);
    rightLayout->addWidget(sendButton);
    rightLayout->addStretch(1);

    auto widgetLayout = new QHBoxLayout();
    widgetLayout->addWidget(_receivedData);
    widgetLayout->addLayout(rightLayout);

    frame->setLayout(widgetLayout);
}

void MainWindow::sendRequest()
{
    FtpAdapter::instance().list();
}

void MainWindow::error()
{
    QTextStream out(stdout);
    out << _ftp->errorString() << Qt::endl;
}

void MainWindow::listDirectory(std::vector<QUrlInfo> info)
{
    QString text;
    QTextStream textStream(&text);
    for (const auto& iter : info) {
        textStream << iter.name() << Qt::endl;
    }
    _receivedData->setText(text);
}

void MainWindow::connected()
{
}

MainWindow::~MainWindow()
{
}

