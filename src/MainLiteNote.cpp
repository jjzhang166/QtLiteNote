#include "QLiteNoteWindow.h"
#include "QtHead.h"
#include "PCString.h"
#include "MkNodePri.h"
#include <stdio.h>
#include <regex>
#include <string>


int ShowLiteNote(int argc, char **argv)
{
    QString str(argv[0]);
#if defined(Q_OS_WIN32)
    // 下面的正则实际上是[\\\/]
    // 因为 \ / 是特殊字符，所以要转义
    int index = str.lastIndexOf(QRegExp("[\\\\\\/]"));
    str = str.mid(0, index);
    //QGridLayout
    
#elif defined(Q_OS_MAC)
    // 在mac中得用.app文件夹封装最后的可执行文件，所有实际的当前路径应该往上返回3层
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

    //printf("%s\n", str.toLocal8Bit().data());

    QTextCodec *c = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(c);

    QApplication app(argc, argv);
    QLiteNoteWindow *dlg = new QLiteNoteWindow(str);

    dlg->show();

    return app.exec();
}

void WriteMdToHtml(const QString &md, const QString &path);
char *ConvertMarkdown(char **strs, int count);

void TestMkNode()
{
	QFile file("D:\\py.txt");
	file.open(QIODevice::ReadOnly);
	QTextStream text(&file);
	text.setCodec("UTF-8");

	std::vector<std::string> lines;
	while (!text.atEnd()) {
		QString str = text.readLine();
		std::string s(str.toUtf8());
		lines.push_back(s);
	}
	MkSyntax syn(lines);
	syn.Analyse();
	syn.GetMkContent();

	std::stringstream stream;
	{
		QFile f2("d:\\css.txt");
		f2.open(QIODevice::ReadOnly);
		QTextStream css(&f2);
		css.setCodec("UTF-8");
		while (!css.atEnd()) {
			QString str = css.readLine();
			std::string s(str.toUtf8());
			stream << s;
		}
	}
	syn.ToString(stream);

	stream << "</body></html>";

	QString html = QString::fromUtf8(stream.str().c_str());
    WriteMdToHtml(html, QString::fromUtf8("d:\\z_md.html"));
}

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
        size_t len = strlen(p);
        char *str = new char[len+1];
        strcpy(str, p);
        strs[i] = str;
    }
    char *r = ConvertMarkdown(strs, ss.size());

    QString md = QString::fromUtf8(r);
    WriteMdToHtml(md, QString::fromUtf8("d:\\z_md.html"));

    return;
}

void TestRegex()
{
	std::regex pat("^#+");
	std::smatch match;
	std::string text("###   head sdf");
	if (std::regex_search(text, match, pat)) {
		//std::ssub_match pre = match.prefix();
		std::ssub_match suf = match.suffix();

		std::string s1(text.begin(), suf.first);
		std::string s2(suf.first, text.end());
		
		printf("\n");
	}

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
	//TestRegex();
	TestMkNode();
	return 0;
    return ShowLiteNote(argc, argv);
}
