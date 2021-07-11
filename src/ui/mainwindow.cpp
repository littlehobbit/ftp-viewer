#include <iostream>

#include <QPushButton>
#include <QBoxLayout>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QMenuBar>
#include <QToolBar>

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->resize(500, 500);

    // Temp signal checking
    _ftp = new FtpAdapter(this);
    QObject::connect(_ftp, &FtpAdapter::errorOccured,
                     this, &MainWindow::error);

    QObject::connect(_ftp, &FtpAdapter::listDone,
                     this, &MainWindow::listDirectory);


    _ftp->connectToHost("ftp.gnu.org");
    _ftp->login();

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
    QBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(_receivedData, 1);
    layout->addWidget(sendButton, 1);

    frame->setLayout(layout);
}

void MainWindow::sendRequest()
{
    _ftp->list();
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

