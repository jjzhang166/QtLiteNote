#pragma once
#include <QtGui>

class QWebView;

class WebForm : public QWidget
{
    Q_OBJECT
public:
    WebForm(QWidget *parent=0);
    ~WebForm(void);

private slots:
    void clickBtn();

private:
    QWebView *m_webview;
};
