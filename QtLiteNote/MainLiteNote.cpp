#include "qapplication.h"
#include "QLiteNoteWindow.h"
#include "qtextcodec.h"

int main(int argc, char **argv)
{
    QTextCodec *c = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForCStrings(c); //����������Ǳ���ģ�������������΢���ź�֮�������
    QTextCodec::setCodecForTr(c);

    QApplication app(argc, argv);
    QLiteNoteWindow *dlg = new QLiteNoteWindow;
    
    //dlg->resize(800, 400);
    dlg->show();

    return app.exec();
}

