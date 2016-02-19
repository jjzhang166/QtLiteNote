#include "Markdown.h"
#include <stdio.h>

//想要在网页中显示中文，得在head头中指定utf-8
//Courier New,
// QWebView不支持指定同时指定英文字体和中文字体，只看指定的第一种字体，所以为了妥协就全部用微软雅黑吧,code中使用Courier New
char *s_head = "<html> <head> <meta http-equiv=\"content-type\" content=\"text/html;charset=UTF-8\">"
"<style type=\"text/css\">"

"body{color: #444444;line-height: 1;padding: 5px;word-wrap: break-word;font-family: DejaVu Sans Mono, \\5FAE\\8F6F\\96C5\\9ED1;}h1, h2, h3, h4 {color: #111111;font-weight:bold;}h1, h2, h3, h4, h5, p {margin-bottom: 16px;padding: 0;} h1 {font-size: 28px;border-bottom: 1px solid;color:#D6615C;}h2 {font-size: 22px;margin: 20px 0 6px;font-weight: bold;color:#2F517B;}h3 {font-size: 18px;font-style:italic;}h4 {font-size: 18px;font-style:italic;}h5 {font-size: 16px;font-style:italic;} a {color: #0099ff;margin: 0;padding: 0;vertical-align: baseline;}a:link,a:visited{text-decoration:none;}a:hover{text-decoration:underline;} p {font-size: 14px;line-height: 20px;margin-top: 3px;}pre {padding: 4px 8px;max-width: 100%white-space: pre-wrap;line-height: 1.5;font-size: 13px;border: 1px solid #ddd;background-color: #f7f7f7;border-radius: 3px;}code {font-family: DejaVu Sans Mono, \\5FAE\\8F6F\\96C5\\9ED1;line-height: 1.5;font-size: 13px;border: 1px solid #ddd;background-color: #f7f7f7;border-radius: 3px;}pre code {border: 0px;}aside {display: block;float: right;width: 100%}blockquote {border-left:.5em solid #40AA53;padding: 0 2em;margin-left:0;max-width: 100%}blockquote cite {font-size:14px;line-height:20px;color:#bfbfbf;}  blockquote p { color: #666;max-width: 100%}hr {height: 1px;border: none;border-top: 1px dashed #0066CC}table {border-spacing: 0;border: 1px solid #ccc;}td, th{border: 1px solid #ccc;padding: 5px;}"
"ul,ol{line-height:24px; margin-left:5px; font-size:14px;}"
//ul, ol {padding: 0;margin: 0;} li {line-height: 24px; margin-left: 44px;} li ul, li ul {margin-left: 24px;} ul, ol {font-size: 14px;line-height: 20px;max-width: 100%}

"</style>"
"</head>"
"<body><p/>\n"
;

/* "body{"
    "background-color:#c7edcc; color: #444444; "
    "line-height: 1; padding: 1px; word-wrap: break-word; font-family:  \\5FAE\\8F6F\\96C5\\9ED1; "
"}"
"p { "
    "font-size: 16px; line-height: 22px;"
    "margin-bottom:20px;"
"}"
"ul, ol {"
    "line-height: 24px;"
    "max-width: 100%"
    "margin-left:30px;"
    "margin-top:0px;"
"}"
"h1 {"
    "font-size: 24px;"
    "line-height: 26px;"
    "border-bottom: 1px solid;"
"}"
"h2 {"
    "font-size: 22px;"
    "font-weight: bold;"
    "color:#2F517B"
"}"
"h3 {"
    "font-size: 20px;"
    "text-decoration: underline;"
"}"
"h4 {"
    "font-size: 18px;"
"}"
"h5 {"
    "font-size: 16px;"
"}"
"pre {"
    "max-width: 100%"
    "white-space: pre-wrap;"
    "line-height: 1;"
    "font-size: 13px;"
    "margin-top:0px;"
    "border: 1px solid #c8c8c8;"
    "background-color: #EBFFE1;"
    "border-radius: 3px;"
"}"
"code {"
    "margin-top:0px;"
    "font-family: Courier New;"
    "line-height: 1.5;"
    "font-size: 13px;"
    "background-color: #EBFFE1;"
    "border-radius: 3px;"
"}"
"pre code {"
    "border: 1px;"
    "margin-top:0px;"
"}" */

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
    /*FILE *file = fopen(QDir::convertSeparators(path).toLocal8Bit().data(), "w");

    if (file) {
        QByteArray arr = md.toUtf8();
        fwrite(arr.data(), arr.length(), 1, file);
        fclose(file);
    }*/
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
            
            //WriteMdToHtml(q, QString::fromUtf8("D:\\z_qt.html"));
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
