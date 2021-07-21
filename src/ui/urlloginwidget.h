#ifndef LINKLOGINWIGET_H
#define LINKLOGINWIGET_H

#include <QWidget>

class QFtp;
class QUrl;

class QLineEdit;

class UrlLoginWidget : public QWidget
{
    Q_OBJECT
public:
    explicit UrlLoginWidget(QWidget *parent = nullptr);

signals:
    void connectRequest(const QUrl &url);

private slots:
    void connectButtonClick(bool);

private:
    QLineEdit *_urlLine = nullptr;
    QLineEdit *_loginLine = nullptr;
    QLineEdit *_passwordLine = nullptr;
};

#endif // LINKLOGINWIGET_H
