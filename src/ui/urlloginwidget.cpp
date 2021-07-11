#include <QLineEdit>
#include <QPushButton>
#include <QBoxLayout>
#include <QFormLayout>
#include <QLabel>

#include "urlloginwidget.h"

UrlLoginWidget::UrlLoginWidget(QWidget *parent) : QWidget(parent)
{
//    this->setMinimumSize(300, 200);
//    this->setMaximumSize(500, 200);

    _urlLine = new QLineEdit();
    _loginLine = new QLineEdit();
    _passwordLine = new QLineEdit();
    auto loginButton = new QPushButton(tr("Connect"));

    auto formLayout = new QFormLayout();
    formLayout->addRow(tr("&Url"), _urlLine);
    formLayout->addRow(tr("&Login"), _loginLine);
    formLayout->addRow(tr("&Password"), _passwordLine);

    formLayout->setRowWrapPolicy(QFormLayout::DontWrapRows);
    formLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    formLayout->setFormAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    formLayout->setLabelAlignment(Qt::AlignLeft);



    auto widgetLayout = new QBoxLayout(QBoxLayout::TopToBottom);
    widgetLayout->addLayout(formLayout);
    widgetLayout->addWidget(loginButton);

    this->setLayout(widgetLayout);
}
