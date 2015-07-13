#include "qapplication.h"
#include "QLiteNoteWindow.h"
#include "qtextcodec.h"
#include <stdio.h>

int main(int argc, char **argv)
{
    printf("argv\n");
//    for (int i = 0; i < argc; ++i) {
//        printf("%s\n", argv[i]);
//    }
    QString str(argv[0]);
    int index = str.lastIndexOf("/");
    str = str.mid(0, index);
    
    printf("%s\n", str.toLocal8Bit().data());
    
    printf("argv_end\n");
    QTextCodec *c = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForCStrings(c); //这里的设置是必须的，这样才能设置微软雅黑之类的字体
    QTextCodec::setCodecForTr(c);

    QApplication app(argc, argv);
    QLiteNoteWindow *dlg = new QLiteNoteWindow(str);
    
    //dlg->resize(800, 400);
    dlg->show();

    return app.exec();
}

