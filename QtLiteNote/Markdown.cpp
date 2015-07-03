#include "Markdown.h"
#include "QDir.h"
#include <stdio.h>

//想要在网页中显示中文，得在head头中指定utf-8
static char *s_head = "<html> <head> <meta http-equiv=\"content-type\" content=\"text/html;charset=UTF-8\">"
"<style type=\"text/css\">"
"body{ color: #444444; line-height: 1; padding: 5px; word-wrap: break-word; font-family: DejaVu Sans Mono, 微软雅黑; } p { font-size: 14px; line-height: 22px; margin-top: 3px;margin-bottom:3px; }"
"</style>"
"</head>"
"<body>"
;

static char *s_end = "</body> </html>";

QString ConvertToMarkdown(const QString &txt)
{
    QStringList ls = txt.split("\r\n");
    
    QString body(s_head);

    for (int i = 0; i < ls.size(); ++i) {
        QString &s = ls[i];
        int len = s.length();
        body += s + "<p>";
    }
    body += s_end;
    return body;
}



void WriteMdToHtml(const QString &md, const QString &path)
{
    FILE *file = fopen(QDir::convertSeparators(path).toLocal8Bit().data(), "w");

    if (file) {
        QByteArray arr = md.toUtf8();
        fwrite(arr.data(), arr.length(), 1, file);
        fclose(file);
    }
}

MarkdownThread::MarkdownThread()
: m_is_stoped(false)
{

}

MarkdownThread::~MarkdownThread()
{
    Stop();
    wait();
}

void MarkdownThread::run()
{
    while (!m_is_stoped) {
        bool is_empty;
        QString md;

        m_lock.lock();
        is_empty = m_markdowns.isEmpty();
        if (!is_empty) {
            md = m_markdowns.back();
            m_markdowns.clear();
        }
        m_lock.unlock();

        if (!is_empty) {
            emit ConvertEnd(ConvertToMarkdown(md));
        }
        QThread::msleep(20);
    }
}

void MarkdownThread::InsertMarkdown(const QString&md)
{
    m_lock.lock();
    m_markdowns.clear();
    m_markdowns.push_back(md);
    m_lock.unlock();
}

void MarkdownThread::Stop()
{
    m_is_stoped = true;
}