#include "WebForm.h"
#include "QtWebKit/QtWebKit"

WebForm::WebForm(QWidget *parent)
: QWidget(parent)
{
    m_webview = new QWebView(this);
    //QUrl u()
    const char *str = "<html>"
        "<head>"
        "<meta charset=\"utf-8\">"
        "<head/>"
        "content<p/>"
        //"<img src=\"file://T:\ITKnowledge\baidu.png\">"
        //"<img src=\"http://images0.cnblogs.com/blog2015/51397/201506/131428188327410.png\">"
        "<img src=\"file:///T:/ITKnowledge/baidu.png\">"
        "<html/>";
    //m_webview->setUrl(QUrl("http://www.baidu.com"));
    m_webview->setContent(str);

    QPushButton *btn = new QPushButton(this);
    btn->setText("refresh");
    btn->setGeometry(800, 0, 70, 25);
    connect(btn, SIGNAL(pressed()), this, SLOT(clickBtn()));
    
}

WebForm::~WebForm(void)
{
}

void WebForm::clickBtn()
{
    m_webview->pageAction(QWebPage::Reload);
}