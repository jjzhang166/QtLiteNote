#pragma once

#include "QtHead.h"

//将html写入文件
void WriteMdToHtml(const QString &md, const QString &path);

class MarkdownThread : public QThread
{
    Q_OBJECT
public:
    MarkdownThread();
    ~MarkdownThread();

    void InsertMarkdown(const QVector<QString> &md);
    void Stop();

protected:
    virtual void run();

signals:
    void ConvertEnd(const QString &html, void *P);

private:
    QMutex m_lock;
	// QVector<QString> 第一个存储笔记所在目录名
    QList<QVector<QString> > m_markdowns;
    volatile bool m_is_stoped;

};
