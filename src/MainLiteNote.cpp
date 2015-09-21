#include "qapplication.h"
#include "QLiteNoteWindow.h"
#include "qtextcodec.h"
#include "qstring.h"
#include "qfile.h"
#include "qdir.h"
#include "qtextstream.h"
#include "PCString.h"
#include <stdio.h>


#import <Foundation/Foundation.h>

char *GetNowPath();
char *GetNowPath()
{
    NSString *str = [NSString stringWithFormat:@"xiangism"];
    
    const char *s = [str cStringUsingEncoding:NSUTF8StringEncoding];
    char *ch = (char*)malloc(strlen(s)+1);
    strcpy(ch, s);
    return ch;
}

int ShowLiteNote(int argc, char **argv)
{
    //printf("argv\n");

    QString str(argv[0]);
#if defined(Q_OS_WIN32)
    // 下面的正则实际上是[\\\/]
    // 因为 \ / 是特殊字符，所以要转义
    int index = str.lastIndexOf(QRegExp("[\\\\\\/]"));
    str = str.mid(0, index);
#elif defined(Q_OS_LINUX)
    int index = str.lastIndexOf(QRegExp("[\\\\\\/]"));
    str = str.mid(0, index);
    
#elif defined(Q_OS_MAC)
    int count = 0;
    for (int i = str.length()-1; i >= 0; --i) {
        if (str[i] == '/') {
            ++count;
        }
        if (count == 4) {
            str = str.mid(0, i);
            break;
        }
    }
    
#endif
    
    

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

void ConvertHtmlTag(PCString *str);
// Horizontal Rules
// Hard Line Breaks
// End a line with two or more
// 123
// 65

int main(int argc, char **argv)
{
    //PCString *str = PCStringNewFromPChar("'xbc'", 0);
    //ConvertHtmlTag(str);

   return ShowLiteNote(argc, argv);
    //TestString();
    //TestString2();
    

    //_CrtDumpMemoryLeaks();

    return 0;
}

