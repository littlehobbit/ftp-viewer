#include <iostream>
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->resize(500, 500);

    // Button configuration
    QBoxLayout *_layout = new QBoxLayout(QBoxLayout::TopToBottom, this);

    QPushButton *sendButton = new QPushButton("Send request", this);
    QPushButton *sendButton2 = new QPushButton("Send request", this);
    QObject::connect(sendButton, &QPushButton::pressed,
                     this, &MainWindow::sendRequest);

//     _receivedData = new QTextEdit("Here will be the answner", this);
//     _receivedData->resize(300, 300);

    _layout->addWidget(sendButton2, 1);
    _layout->addSpacing(20);
    _layout->addWidget(sendButton, 2);

    this->setLayout(_layout);
}

void MainWindow::sendRequest()
{
    std::cout << "clicked" <<std::endl;
}

MainWindow::~MainWindow()
{
}

