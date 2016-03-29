#include "Markdown.h"
#include <stdio.h>
#include "MkNodePri.h"

//想要在网页中显示中文，得在head头中指定utf-8
//Courier New,
// QWebView不支持指定同时指定英文字体和中文字体，只看指定的第一种字体，所以为了妥协就全部用微软雅黑吧,code中使用Courier New
const char *s_head = 
"<html> <head> <meta http-equiv=\"content-type\" content=\"text/html;charset=UTF-8\">"
"<style type=\"text/css\">"
"body {color: #444444;line-height: 1;padding: 5pt;word-wrap: break-word;font-family: DejaVu Sans Mono, \\5FAE\\8F6F\\96C5\\9ED1;}"
"h1, h2, h3, h4 {color: #111111;font-weight:bold;}"
"h1, h2, h3, h4, h5, p {margin-bottom: 16pt; padding: 0;} "
"h1 {font-size: 28pt;border-bottom: 1pt solid;color:#D6615C;}"
"h2 {font-size: 22pt;margin: 20pt 0 6pt;font-weight: bold;color:#2F517B;}"
"h3 {font-size: 18pt;font-style:italic;}"
"h4 {font-size: 18pt;font-style:italic;}"
"h5 {font-size: 16pt;font-style:italic;}"
"a {color: #0099ff;margin: 0;padding: 0;vertical-align: baseline;}"
"a:link,a:visited {text-decoration:none;}"
"a:hover{text-decoration:underline;}"
"p {font-size: 14pt;line-height: 20pt;margin-top: 3pt;}"
"pre {padding: 4pt 8pt;max-width: 100%white-space: pre-wrap;line-height: 1.5;font-size: 13pt;border: 1pt solid #ddd;background-color: #f7f7f7;border-radius: 3pt;}"
"code {font-family: DejaVu Sans Mono, \\5FAE\\8F6F\\96C5\\9ED1;line-height: 1.5;font-size: 13pt;border: 1pt solid #ddd;background-color: #f7f7f7;border-radius: 3pt;}"
"pre code {border: 0pt;}"
"aside {display: block;float: right;width: 100%}"
"blockquote {border-left:.5em solid #40AA53;padding: 0 2em;margin-left:0;max-width: 100%}"
"blockquote cite {font-size:14pt;line-height:20pt;color:#bfbfbf;}"
"blockquote p { color: #666;max-width: 100%}"
"hr {height: 1pt;border: none;border-top: 1pt dashed #0066CC}"
"table {border-spacing: 0;border: 1pt solid #ccc;}"
"td,th{border: 1pt solid #ccc;padding: 5pt;}"
"ul,ol{line-height:24pt; margin-left:5pt; font-size:14pt;}"
"</style>"
"</head>"
"<body><p/>\n"
;


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
    FILE *file = fopen(QDir::toNativeSeparators(path).toLocal8Bit().data(), "w");

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

        /*if (!is_empty) {
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
            emit ConvertEnd(q);
            //WriteMdToHtml(q, QString::fromUtf8("D:\\temp_mk.html"));
        }*/
		if (!is_empty) {
			std::vector<std::string> lines;
			for (int i = 0; i < md.size(); ++i) {
				std::string s(md[i].toUtf8());
				lines.push_back(s);
			}
			MkSyntax syn(lines);
			syn.Analyse();
			syn.GetMkContent();
			std::stringstream stream;
			{
				QFile f2("d:\\css.txt");
				f2.open(QIODevice::ReadOnly);
				QTextStream css(&f2);
				css.setCodec("UTF-8");
				while (!css.atEnd()) {
					QString str = css.readLine();
					std::string s(str.toUtf8());
					stream << s;
				}
			}
			syn.ToString(stream);

			stream << "</body></html>";
			QString html = QString::fromUtf8(stream.str().c_str());
			emit ConvertEnd(html);
            WriteMdToHtml(html, QString::fromUtf8("D:\\temp_mk.html"));
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
