#include "qapplication.h"
#include "QLiteNoteWindow.h"
#include "qtextcodec.h"
#include <stdio.h>

int main(int argc, char **argv)
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

