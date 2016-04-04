#include "MarkdownThread.h"
#include <stdio.h>
#include "MkNodePri.h"

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

		if (!is_empty) {
			std::vector<std::string> lines;

			if (md.size() >= 1) {
				QString dir = md[0];
				for (int i = 1; i < md.size(); ++i) {
					std::string s(md[i].toUtf8());
					lines.push_back(s);
				}
                std::pair<std::string, AnchorNode*> r = SyntaxMk(lines, std::string(dir.toUtf8()), 10);
				QString html = QString::fromUtf8(r.first.c_str());
				emit ConvertEnd(html, r.second);
			}

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
