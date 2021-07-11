#ifndef LINKLOGINWIGET_H
#define LINKLOGINWIGET_H

#include <QWidget>

class QLineEdit;

class UrlLoginWidget : public QWidget
{
    Q_OBJECT
public:
    explicit UrlLoginWidget(QWidget *parent = nullptr);

signals:

private:
    QLineEdit *_urlLine = nullptr;
    QLineEdit *_loginLine = nullptr;
    QLineEdit *_passwordLine = nullptr;

};

#endif // LINKLOGINWIGET_H
