#include "Markdown.h"
#include "qdir.h"
#include <stdio.h>

//想要在网页中显示中文，得在head头中指定utf-8
static char *s_head = "<html> <head> <meta http-equiv=\"content-type\" content=\"text/html;charset=UTF-8\">"
"<style type=\"text/css\">"
"body{ color: #444444; line-height: 1; padding: 5px; word-wrap: break-word; font-family: DejaVu Sans Mono, \\5FAE\\8F6F\\96C5\\9ED1; } p { font-size: 14px; line-height: 22px; margin-top: 3px;margin-bottom:3px; }"
"</style>"
"</head>"
"<body><p/>"
;

//DejaVu Sans Mono, 

static char *s_end = "</body> </html>";

int GetHead(QString &str)
{
    for (int i = 0; i < str.size(); ++i) {
        if (str[i] != '#') {
            str = str.mid(i);
            return i;
        }
        if (i == 6) {
            return 6;
        }
    }
    return 0;
}

void ConvertHead(QString &str)
{
    if (str.isEmpty()) {
        return;
    }
    if (str[0] == '#') {
        int h = GetHead(str);
        
        if (!str.isEmpty() && h >= 1 && h <= 6) {
            char s1[10], s2[10];
            sprintf(s1, "<h%d>", h);
            sprintf(s2, "</h%d>", h);

            str = s1 + str + s2;
        }
    }    
}

QString ConvertToMarkdown(const QVector<QString> &ls)
{
    //QStringList ls = txt.split("\r\n");
    
    QString body = QString::fromUtf8(s_head);

    for (int i = 0; i < ls.size(); ++i) {
        QString s = ls[i];

        if (s.isEmpty()) {
            body += "<p/>";
            continue;
        } 
        s.replace('&', "&amp;");
        s.replace('<', "&lt;");
        s.replace('>', "&gt;");
        s.replace('"', "&quot;");
        s.replace('\'', "&qpos;");

        if (s[0] == '#') {
            ConvertHead(s);
        } else {
            s.replace(' ', "&nbsp;");
        }

        body += s + "<p/>";
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
        QVector<QString> md;

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

void MarkdownThread::InsertMarkdown(const QVector<QString> &md)
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
