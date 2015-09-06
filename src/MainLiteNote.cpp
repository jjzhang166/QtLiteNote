#include "qapplication.h"
#include "QLiteNoteWindow.h"
#include "qtextcodec.h"
#include "qstring.h"
#include "qfile.h"
#include "qtextstream.h"
#include "PCString.h"
#include <stdio.h>

int ShowLiteNote(int argc, char **argv)
{
    //printf("argv\n");

    QString str(argv[0]);

    // 下面的正则实际上是[\\\/]
    // 因为 \ / 是特殊字符，所以要转义
    int index = str.lastIndexOf(QRegExp("[\\\\\\/]"));
    str = str.mid(0, index);

    printf("%s\n", str.toLocal8Bit().data());

    QTextCodec *c = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForCStrings(c); //这里的设置是必须的，这样才能设置微软雅黑之类的字体
    QTextCodec::setCodecForTr(c);

    QApplication app(argc, argv);
    QLiteNoteWindow *dlg = new QLiteNoteWindow(str);

    //dlg->resize(800, 400);
    dlg->show();

    return app.exec();
}

void WriteMdToHtml(const QString &md, const QString &path);
char *ConvertMarkdown(char **strs, int count);

void TestString()
{
    QFile file("D:\\new.txt");
    file.open(QIODevice::ReadOnly);
    QTextStream text(&file);
    text.setCodec("UTF-8");

    QVector<QString> ss;
    while (!text.atEnd()) {
        ss.push_back(text.readLine());
    }
    char **strs = new char *[ss.size()];
    for (int i = 0; i < ss.size(); ++i) {
        
        QByteArray arr = ss[i].toUtf8();
        char *p = arr.data();
        int len = strlen(p);
        char *str = new char[len+1];
        strcpy(str, p);
        strs[i] = str;
    }
    char *p = strs[ss.size()-1];
    char *r = ConvertMarkdown(strs, ss.size());

    QString md = QString::fromUtf8(r);
    WriteMdToHtml(md, QString::fromUtf8("d:\\z_md.html"));

    return;
}

int main(int argc, char **argv)
{

   return ShowLiteNote(argc, argv);
    //TestPCString();
    //TestString();
    

    _CrtDumpMemoryLeaks();

    return 0;
}

