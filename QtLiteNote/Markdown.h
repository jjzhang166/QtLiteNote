#pragma once

#include "qstring.h"
#include "qthread.h"
#include "qlist.h"
#include "qmutex.h"

//�����ı�ת��Ϊhtml��ʽ������html����β���ֺ�css��ʽ
QString ConvertToMarkdown(const QString &txt);

//��htmlд���ļ�
void WriteMdToHtml(const QString &md, const QString &path);

class MarkdownThread : public QThread
{
    Q_OBJECT
public:
    MarkdownThread();
    ~MarkdownThread();

    void InsertMarkdown(const QString&md);
    void Stop();

protected:
    virtual void run();

signals:
    void ConvertEnd(const QString &html);

private:
    QMutex m_lock;
    QList<QString> m_markdowns;
    volatile bool m_is_stoped;

};