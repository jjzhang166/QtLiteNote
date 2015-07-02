#include "qapplication.h"
#include "QLiteNoteWindow.h"
#include "qtextcodec.h"

int main(int argc, char **argv)
{
    QTextCodec *c = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForCStrings(c); //这里的设置是必须的，这样才能设置微软雅黑之类的字体
    QTextCodec::setCodecForTr(c);

    QApplication app(argc, argv);
    QLiteNoteWindow *dlg = new QLiteNoteWindow;
    
    //dlg->resize(800, 400);
    dlg->show();

    return app.exec();
}

