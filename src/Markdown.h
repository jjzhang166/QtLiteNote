#pragma once

#include <QtCore/QString>
#include <QtCore/QThread>
#include <QtCore/QList>
#include <QtCore/QVector>
#include <QtCore/QMutex>

//将纯文本转化为html格式，加入html的首尾部分和css样式
//QString ConvertToMarkdown(const QString &txt);

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
    void ConvertEnd(const QString &html);

private:
    QMutex m_lock;
    QList<QVector<QString> > m_markdowns;
    volatile bool m_is_stoped;

};
