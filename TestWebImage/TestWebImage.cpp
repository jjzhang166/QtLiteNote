//#include <windows.h>
#include "WebForm.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    WebForm form;
    form.resize(1000, 650);
    form.show();
	return app.exec();
}

