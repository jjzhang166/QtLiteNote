#include "Markdown.h"
#include "qdir.h"
#include <stdio.h>

//想要在网页中显示中文，得在head头中指定utf-8
char *s_head = "<html> <head> <meta http-equiv=\"content-type\" content=\"text/html;charset=UTF-8\">"
"<style type=\"text/css\">"
"body{background-color:#c7edcc; color: #444444; "
"line-height: 1; padding: 1px; word-wrap: break-word; font-family: Courier New; } "
"p { "
"font-size: 14px; line-height: 25px;"
"margin-top: 3px;margin-bottom:3px; }"
"ul, ol {"
"font-size: 14px;"
"line-height: 22px;"
"max-width: 100%"
"margin-left:30px;"
"}"
"h1 {"
"font-size: 28px;"
"line-height: 35px;"
"border-bottom: 1px solid;"
"}"
"h2 {"
"font-size: 22px;"
"font-weight: bold;"
"color:#2F517B"
"}"
"h3 {"
"font-size: 21px;"
"text-decoration: underline;"
"}"
"h4 {"
"font-size: 18px;"
"}"
"h5 {"
"font-size: 16px;"
"}"
"pre {"
"padding: 0px 4px;"
    "max-width: 100%"
        "white-space: pre-wrap;"
    "line-height: 1;"
    "font-size: 13px;"
"border: 1px solid #c8c8c8;"
    "background-color: #EBFFE1;"
    "border-radius: 3px;"
"}"
"code {"
    "font-family: Courier New;"
    "line-height: 1.5;"
    "font-size: 13px;"
    "background-color: #EBFFE1;"
    "border-radius: 3px;"
"}"
"pre code {"
"border: 1px;"
"}"
"</style>"
"</head>"
"<body><p/>\n"
;

/*char *s_head = "<html> <head> <meta http-equiv=\"content-type\" content=\"text/html;charset=UTF-8\">"
"</head>"
"<body><p/>\n"
;*/


// DejaVu Sans Mono, 
// Courier New

// 中国人的
// 1122iiMM

/*int GetHead(QString &str)
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

bool IsCode(QString &str)
{
    if (str.size() == 3) {
        return str == "```";
    }
    return false;
}

QString ConvertToMarkdown(const QVector<QString> &ls)
{
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
        s.replace('\'', "&qpos;"); // 单引号不需要替换

        if (s[0] == '#') {
            ConvertHead(s);
        } else if (IsCode(s)) {
            
            
        } else {
            s.replace(' ', "&nbsp;");
        }

        body += s + "<p/>";
    }
    body += s_end;
    return body;
}*/

char *ConvertMarkdown(char **strs, int count);

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
            char **strs = (char**)malloc(md.size() * sizeof(char*));
            for (int i = 0; i < md.size(); ++i) {
               QString &s = md[i];
               QByteArray arr = s.toUtf8();
               const char *p = arr.data();
               int len = strlen(p);
               strs[i] = (char*)malloc(len+1);
               memcpy(strs[i], p, len);
               strs[i][len] = '\0';
            }
            char *p = ConvertMarkdown(strs, md.size());
            QString q = QString::fromUtf8(p);
            free(p);
            for (int i = 0; i < md.size(); ++i) {
                free(strs[i]);
            }
            free(strs);
            
            //emit ConvertEnd(ConvertToMarkdown(md));
            WriteMdToHtml(q, QString::fromUtf8("D:\\z_qt.html"));
            emit ConvertEnd(q);
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
