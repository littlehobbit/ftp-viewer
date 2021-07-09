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

    _ftp = new QFtp(this);
    // ftp logging
    QObject::connect(_ftp, &QFtp::commandFinished,
                     this, [this] (int id, bool error) {
        QTextStream out(stdout);
        out << id << ": ";
        if (error) {
            out << "error - " << _ftp->errorString();
        } else {
            out << "done";
            if (_ftp->currentCommand() == QFtp::List) {
                out << " list was proccessed";
            }
        }
        out << Qt::endl;
        std::cout << "======================================" << std::endl;
    });

    QObject::connect(_ftp, &QFtp::listInfo, this, [](const QUrlInfo& info) {
        QTextStream out(stdout);
        out << info.name() << Qt::endl;
    });

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

    // Network manager
    _networkManager = new QNetworkAccessManager(this);
    connect(_networkManager, &QNetworkAccessManager::finished,
            this, [this](QNetworkReply *reply) {
        if (reply->error() != QNetworkReply::NoError) {
            QTextStream out(stdout);
            out << "Error: " << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() << Qt::endl;
        } else {
            _receivedData->setText(reply->readAll());
        }

        reply->deleteLater();
    });
}

void MainWindow::sendRequest()
{
    _ftp->list();
    _ftp->list("pub");
    _ftp->list("video");
}

MainWindow::~MainWindow()
{
}

