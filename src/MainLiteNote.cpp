#include "QLiteNoteWindow.h"
#include "QtHead.h"
#include "PCString.h"
#include "MkNodePri.h"
#include <stdio.h>
#include <regex>
#include <string>

std::string ltrim(const std::string &ss);

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

    QTextCodec *c = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(c);

    QApplication app(argc, argv);
    QLiteNoteWindow *dlg = new QLiteNoteWindow(str);

    dlg->show();

    return app.exec();
}

void WriteMdToHtml(const QString &md, const QString &path);
//char *ConvertMarkdown(char **strs, int count);

std::vector<std::string> ReadMkUTF8(const char *filename)
{
	QFile file(filename);
	file.open(QIODevice::ReadOnly);
	QTextStream text(&file);
	text.setCodec("UTF-8");

	std::vector<std::string> lines;
	while (!text.atEnd()) {
		QString str = text.readLine();
		std::string s(str.toUtf8());
		lines.push_back(s);
	}
	return lines;
}

void TestMkNode()
{
	std::vector<std::string> lines = ReadMkUTF8("D:\\base.txt");
	MkSyntax syn(lines, "");
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

void ScanAnchor(AnchorNode *node, int level)
{
	printf("Anchor:");
	for (int i = 0; i < node->m_level; ++i) {
		printf(" ");
	}
	//printf("%d, %s\n", node->m_level, node->m_name.c_str());
	printf("%d", node->m_level);
	qDebug() << QString::fromUtf8(node->m_name.c_str());

	for (int i = 0; i < node->m_children.size(); ++i) {
		ScanAnchor(node->m_children[i], level + 1);
	}
}

void TestMkNode2()
{
	std::vector<std::string> lines = ReadMkUTF8("D:\\test.txt");
    //std::vector<std::string> lines = ReadMkUTF8("/Users/xiangbc/test.txt");

    std::pair<std::string, AnchorNode*> r = SyntaxMk(lines, "T:\\ITKnowledge\\Language\\python", 18);
	QString html = QString::fromUtf8(r.first.c_str());
	WriteMdToHtml(html, QString::fromUtf8("d:\\z_md.html"));

	ScanAnchor(r.second, 0);
	ReleaseAnchorNode(r.second);
}

void TestLeftTrim()
{
	std::string s(" 向百123");
	s = ltrim(s);

	printf("\n");
}

void TestRegex()
{
	std::regex pat("^#+");
	std::smatch match;
	std::string text("###   head sdf");
	if (std::regex_search(text, match, pat)) {
		std::ssub_match suf = match.suffix();

        std::string s1(text.begin(), text.end());
//        std::string s2(suf.begin(), text.end());
		
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
    //ln::LExceptionHandle *e = ln::LExceptionHandle::GetInstance();
    //e->Start();

    //TestLeftTrim();
    //TestRegex();
    //TestMkNode();
    //TestMkNode2();
    //return 0;
    return ShowLiteNote(argc, argv);
}
