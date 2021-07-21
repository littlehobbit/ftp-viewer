#include <iostream>

#include <QLineEdit>
#include <QPushButton>
#include <QBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QTextStream>
#include <QUrl>

#include <QFtp>

#include "urlloginwidget.h"

UrlLoginWidget::UrlLoginWidget(QWidget *parent) : QWidget(parent)
{
    _urlLine = new QLineEdit("ftp.gnu.org");
    _loginLine = new QLineEdit("anonymous");
    _passwordLine = new QLineEdit();
    auto connectButton = new QPushButton(tr("Connect"));

    _urlLine->setPlaceholderText(tr("Url to ftp server"));
    _loginLine->setPlaceholderText(tr("Login"));
    _passwordLine->setPlaceholderText(tr("Password"));

    auto formLayout = new QVBoxLayout();
    formLayout->addWidget(_urlLine);
    formLayout->addWidget(_loginLine);
    formLayout->addWidget(_passwordLine);
    formLayout->addWidget(connectButton);

    this->setLayout(formLayout);

    // signal & slot configuration
    QObject::connect(connectButton, &QPushButton::clicked,
                     this, &UrlLoginWidget::connectButtonClick);
}

void UrlLoginWidget::connectButtonClick(bool checked)
{
    const QString url = QString("//%1:%2@%3").arg(_loginLine->text(),
                                          _passwordLine->text(),
                                          _urlLine->text());
    const QUrl ftpPath(url);

    emit connectRequest(ftpPath);
}
