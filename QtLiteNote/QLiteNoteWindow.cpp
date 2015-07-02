#include "QtGui"
#include "QtWebKit"
#include "QLiteNoteWindow.h"
#include "QTreeWidgetEx.h"
#include "markdown.h"

#if defined(Q_OS_WIN32)

#include <windows.h>
int ShellExecute(const char* path)
{
    SHELLEXECUTEINFOA info;
    memset(&info, 0, sizeof(info));
    info.cbSize = sizeof(info);
    info.lpFile = path;
    info.lpParameters = "";
    info.nShow = SW_SHOWNORMAL;
    ::ShellExecuteExA(&info);
    return 0;
}
#endif

// src, dst都是完整的文件路径
// 比如将 src=d:\1.txt 移到dst=e:\temp\2.txt
// 如果已经存在文件则直接删除
// dst所在的文件夹不必存在
static bool QCopyFile(const QString &src, const QString &dst)
{
    if (src == dst) {
        return true;
    }
    if (!QFile::exists(src)) {
        return false;
    }
    if (QFile::exists(dst)) {
        QFile::remove(dst);
    }
    QFileInfo info(dst);
    QDir dir;
    QString t = info.absolutePath();
    dir.mkpath(t);
    return QFile::copy(src, dst);
}

//src_path, dst_path都是完整的文件夹路径
// 比如将 src=d:\dir 移到 dst=e:\path
//如果已经存在文件夹则合并起来
bool QCopyDir(const QString &src_path, const QString &dst_path)
{
    return true;
}

QLiteNoteWindow::QLiteNoteWindow(QWidget *parent)
    : QMainWindow(parent),
    m_split(NULL),
    m_tree(NULL),
    m_edit_box(NULL),
    m_webview(NULL),
    m_now_item(NULL),
    m_tree_font("微软雅黑", 10),
    m_dir_icon(":/ras/dir.png"),
    m_dir_open_icon(":/ras/dir_open.png"),
    m_note_icon(":/ras/note.png"),
    m_dir_menu(NULL),
    m_note_menu(NULL),
    m_black_menu(NULL),
    m_new_note_action(NULL),
    m_new_dir_action(NULL),
    m_edit_action(NULL),
    m_open_explorer_action(NULL),
    m_refresh_action(NULL),
    m_new_root_action(NULL),
    m_path_label(NULL),
    m_file_menu(NULL),
    m_edit_menu(NULL),
    m_options_menu(NULL),
    m_help_menu(NULL),
    m_exit_action(NULL),
    m_about_action(NULL),
    m_thread(NULL)
{
    CreateAction();
    CreateMenu();
    CreateStatus();

    m_tree = new QTreeWidgetEx(this);
    m_tree->setHeaderHidden(true);
    m_tree->setRootIsDecorated(true);
    m_tree->setVerticalScrollMode(QTreeWidget::ScrollPerPixel);
    //m_tree->setEditTriggers(QTreeWidget::CurrentChanged);

    connect(m_tree, SIGNAL(itemSelect(QTreeWidgetItem*)), this, SLOT(TreeItemSelect(QTreeWidgetItem*)));
    connect(m_tree, SIGNAL(itemDelete(QTreeWidgetItem*)), this, SLOT(TreeItemDelete(QTreeWidgetItem*)));
    connect(m_tree, SIGNAL(rightClick(QTreeWidgetItem*)), this, SLOT(TreeRightClick(QTreeWidgetItem*)));
    connect(m_tree, SIGNAL(doubleClick(QTreeWidgetItem*)), this, SLOT(TreeItemDoubleClick(QTreeWidgetItem*)));
    connect(m_tree, SIGNAL(itemExpanded(QTreeWidgetItem *)), this, SLOT(TreeItemExpand(QTreeWidgetItem *)));
    connect(m_tree, SIGNAL(itemCollapsed(QTreeWidgetItem*)), this, SLOT(TreeItemCollapsed(QTreeWidgetItem*)));

    m_webview = new QWebView(this);

    m_split = new QSplitter(Qt::Horizontal);
    m_split->addWidget(m_tree);
    m_split->addWidget(m_webview);
    m_split->setStretchFactor(1, 1);

    m_thread = new MarkdownThread;
    connect(m_thread, SIGNAL(ConvertEnd(const QString &)), this, SLOT(ConvertEnd(const QString&)));
    m_thread->start();

    ReadSettings();
    UpdateStatue();

    setWindowIcon(QIcon(":/ras/app.png"));
    setCentralWidget(m_split);

    QString path("G:\\txtNote");
    RefreshRoot(path);
    WebBlack();

    m_note_path = path;
    m_trash_path = m_note_path + QDir::separator() + "trash";
    QDir dir(path);
    dir.mkdir("trash");
}

QLiteNoteWindow::~QLiteNoteWindow(void)
{
}

void QLiteNoteWindow::closeEvent(QCloseEvent *event)
{
    WriteSettings();
}

void QLiteNoteWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
        case Qt::Key_F5:
            printf("Window_F5\n");
            m_webview->pageAction(QWebPage::Reload);
            break;

        case Qt::Key_Delete:
            printf("Win_Delete\n");
            break;
    }
  
}

void QLiteNoteWindow::CreateAction()
{
    //
    m_exit_action = new QAction(tr("E&xit"), this);
    connect(m_exit_action, SIGNAL(triggered()), this, SLOT(close()));

    m_about_action = new QAction(QString::fromUtf8("关于"), this);
    m_about_action->setIcon(QIcon(":/ras/note.png"));
    connect(m_about_action, SIGNAL(triggered()), this, SLOT(ShowAbout()));

    //
    m_new_note_action = new QAction(tr("NewNote"), this);
    m_new_note_action->setIcon(m_note_icon);
    connect(m_new_note_action, SIGNAL(triggered()), this, SLOT(AddNewNote()));

    m_new_dir_action = new QAction(tr("NewDir"), this);
    m_new_dir_action->setIcon(m_dir_icon);
    connect(m_new_dir_action, SIGNAL(triggered()), this, SLOT(AddNewDir()));

    m_edit_action = new QAction(tr("EditNote"), this);
    connect(m_edit_action, SIGNAL(triggered()), this, SLOT(EditNote()));

    m_open_explorer_action = new QAction(tr("open explorer"), this);
    m_open_explorer_action->setIcon(QIcon(":/ras/computer.png"));
    connect(m_open_explorer_action, SIGNAL(triggered()), this, SLOT(OpenExplorer()));

    m_new_root_action = new QAction(tr("添加目录"), this);
    connect(m_new_root_action, SIGNAL(triggered()), this, SLOT(NewRootDir()));

    m_refresh_action = new QAction("全部刷新", this);
    connect(m_refresh_action, SIGNAL(triggered()), this, SLOT(RefreshAll()));
}


void QLiteNoteWindow::CreateMenu()
{
    //top menu
    m_file_menu = menuBar()->addMenu(tr("&File"));
    m_file_menu->addAction(m_new_note_action);
    m_file_menu->addAction(m_new_dir_action);
    m_file_menu->addSeparator();
    m_file_menu->addAction(m_exit_action);

    m_edit_menu = menuBar()->addMenu(tr("&Edit"));

    m_options_menu = menuBar()->addMenu(tr("&Options"));

    m_help_menu = menuBar()->addMenu(tr("&Help"));
    m_help_menu->addAction(m_about_action);


    //context menu
    m_dir_menu = new QMenu(this);
    m_dir_menu->addAction(m_new_note_action);
    m_dir_menu->addAction(m_new_dir_action);
    m_dir_menu->addSeparator();
    m_dir_menu->addAction(m_open_explorer_action);

    m_note_menu = new QMenu(this);
    m_note_menu->addAction(m_edit_action);
    m_note_menu->addSeparator();
    m_note_menu->addAction(m_open_explorer_action);

    m_black_menu = new QMenu(this);
    m_black_menu->addAction(m_new_root_action);
    m_black_menu->addSeparator();
    m_black_menu->addAction(m_refresh_action);
    
}

void QLiteNoteWindow::CreateStatus()
{
    m_path_label = new QLabel(tr("  "));
    m_path_label->setAlignment(Qt::AlignLeft);
    
    statusBar()->addWidget(m_path_label, 1);
}

void QLiteNoteWindow::RefreshRoot(const QString &path)
{
    QDir dir(path);
    QStringList filters;
    QStringList fs = dir.entryList(filters, QDir::Dirs);

    int c = m_tree->topLevelItemCount();
    for (int i = 0; i < c; ++i) {
        QTreeWidgetItem *item = m_tree->topLevelItem(i);
        ClearNode(item);
    }
    m_tree->clear();
    m_tree->m_now_select_node = NULL;
    m_now_item = NULL;

    for (int i = 2; i < fs.size(); ++i) {
        QString s = fs.at(i);
        if (s.toLower() == "trash") {
            continue;
        }
        QString full_path = path + QDir::separator() + s;
        //QByteArray arr = s.toLocal8Bit();
        //printf("%s\n", arr.data());

        QStringList name;
        name.push_back(s);
        QTreeWidgetItem *d = new QTreeWidgetItem(name);
        d->setIcon(0, QIcon(":/ras/dir.png"));
        d->setData(1, 0, full_path);
        d->setFont(0, m_tree_font);
        d->setFlags(Qt::ItemIsEditable|Qt::ItemIsSelectable|Qt::ItemIsEnabled);

        m_tree->insertTopLevelItem(m_tree->topLevelItemCount(), d);

        RefreshNode(d, full_path);
    }

    if (m_tree->topLevelItemCount() != 0) {
        m_tree->SetSelectItem(m_tree->topLevelItem(0));
    }
}

void QLiteNoteWindow::RefreshNode(QTreeWidgetItem *node, const QString &path, bool scan_child_dir)
{
    QDir dir(path);
    QStringList filters;
    QStringList dirs = dir.entryList(filters, QDir::Dirs);

    std::list<QString> t1 = dirs.toStdList();
    
    filters.push_back("*.txt");
    filters.push_back("*.html");

    QStringList files = dir.entryList(filters, QDir::Files);

    for (int i = 2; i < dirs.size(); ++i) {
        QString ss = path+QDir::separator()+dirs[i];
        QStringList name;
        name.push_back(dirs[i]);

        QTreeWidgetItem *d = new QTreeWidgetItem(name);
        d->setIcon(0, m_dir_icon);
        d->setData(1, 0, ss);
        d->setFont(0, m_tree_font);
        
        node->insertChild(node->childCount(), d);

        if (scan_child_dir) {
            RefreshNode(d, ss, false);
        }
    }

    for (int i = 0; i < files.size(); ++i) {
        QStringList name;
        name.push_back(files[i]);
        QTreeWidgetItem *d = new QTreeWidgetItem(name);
        d->setIcon(0, m_note_icon);
        d->setFont(0, m_tree_font);
        d->setData(1, 0, path+QDir::separator()+files[i]);
        node->insertChild(node->childCount(), d);
    }
}

void QLiteNoteWindow::TreeItemSelect(QTreeWidgetItem *item)
{
    if (item) {
        m_now_item = item;
        UpdateStatue();

        QString s = item->data(1, 0).toString();
        QFileInfo f(s);

        if (f.isFile()) {
            ShowNote(s);
        } 
    }
}

void QLiteNoteWindow::TreeItemDelete(QTreeWidgetItem *item)
{
    int r = QMessageBox::warning(this, "QtLiteNote", QString::fromUtf8("确定要删除吗?"), QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
    if (r == QMessageBox::Yes) {
        QString src = item->data(1, 0).toString();
        QString name = src.mid(m_note_path.size()+1);
        QFileInfo file(src);

        if (file.isFile()) {
            QString t = file.fileName();
            QCopyFile(src, m_trash_path + QDir::separator() + name);
            QFile::remove(src);

        } else if (file.isDir()) {
            //TODO:删除文件夹

        }

        if (item->parent()) {
            m_tree->SetSelectItem(item->parent());
            TreeItemExpand(item->parent());
        } else {

        }        
       
        return;
    }
}

void QLiteNoteWindow::TreeRightClick(QTreeWidgetItem *item)
{
    if (item) {
        m_now_item = item;
        UpdateStatue();

        QString s = item->data(1, 0).toString();
        QFileInfo file(s);
        if (file.isFile()) {
            //m_note_menu->exec(this->mapToParent(QPoint(x, y)));
            m_note_menu->exec(cursor().pos());
        } else if (file.isDir()) {
            m_dir_menu->exec(cursor().pos());
        }

    } else {
        m_black_menu->exec(cursor().pos());
    }
}

void QLiteNoteWindow::ClearNode(QTreeWidgetItem *node)
{
    int c = node->childCount(); //这里的c是个肯定值，不能放到for循环的()中去

    for (int i = 0; i < c; ++i) {
        QTreeWidgetItem *d = node->child(0);
        ClearNode(d);
        node->removeChild(d);

        delete d;
    }
}

void QLiteNoteWindow::TreeItemDoubleClick(QTreeWidgetItem *item)
{
    if (item) {
        QString s = item->data(1, 0).toString();
        EditNote(s);
    }
}

void QLiteNoteWindow::TreeItemCollapsed(QTreeWidgetItem *item)
{
    if (item) {
        item->setIcon(0, m_dir_icon);
    }
}

void QLiteNoteWindow::TreeItemExpand(QTreeWidgetItem *item)
{
    if (item) {
        item->setIcon(0, m_dir_open_icon);

        //将同级的所有节点都收起来
        //由于不能用相同的代码处理根节点和子节点，所以要先判断
        //是不是考虑将这个功能移到QTreeWidgetEx类中
        if (item->parent()) {

            for (int i = 0; i < item->parent()->childCount(); ++i) {
                QTreeWidgetItem *it = item->parent()->child(i);

                if (it != item) {
                    it->setExpanded(false);
                }
            }
        } else {
            for (int i = 0; i < m_tree->topLevelItemCount(); ++i) {
                QTreeWidgetItem *it = m_tree->topLevelItem(i);

                if (it != item) {
                    it->setExpanded(false);
                }
            }
        }
        
        ClearNode(item);

        QString path = item->data(1, 0).toString();
        RefreshNode(item, path, true);
    }
}

void QLiteNoteWindow::ExpandAndSelectNew(const QString &path)
{
    if (m_now_item) {
        if (m_now_item->isExpanded()) {
            m_now_item->setExpanded(false);
            m_now_item->setExpanded(true);
        } else {
            m_now_item->setExpanded(true);
        }
        //TODO:选中新节点
        for (int i = 0; i < m_now_item->childCount(); ++i) {
            QString s = m_now_item->child(i)->data(1, 0).toString();

            if (s == path) {
                m_tree->SetSelectItem(m_now_item->child(i));
            }
        }
        
    }
}

QString QLiteNoteWindow::FindNewFileName(const QString &path, const QString &post)
{
    QString temp = path + QDir::separator() + "new" + post;
    QFileInfo f(temp);
    if (!f.exists())
        return temp;

    int i = 2;
    while (true) {
        temp = path + QDir::separator() + "new_" + QString::number(i) + post;

        QFileInfo f(temp);
        if (!f.exists()) {
            return temp;
        }
        ++i;
    }
}

void QLiteNoteWindow::AddNewNote()
{
    if (m_now_item) {
        QString path = m_now_item->data(1, 0).toString();

        QString new_path = FindNewFileName(path, QString(".txt"));
        printf("NewNote: %s\n", new_path.toLocal8Bit().data());
        
        FILE *file = fopen(new_path.toLocal8Bit().data(), "wt");
        fclose(file);

        ExpandAndSelectNew(new_path);
    }
}

void QLiteNoteWindow::AddNewDir()
{
    if (m_now_item) {
        QString path = m_now_item->data(1, 0).toString();
        QString new_path = FindNewFileName(path, QString(""));
        printf("NewDir: %s\n", new_path.toLocal8Bit().data());
        QDir dir;
        dir.mkdir(new_path);
        
        ExpandAndSelectNew(new_path);
    }

}

void QLiteNoteWindow::ConvertEnd(const QString &html)
{
     //TODO:在这里加入多线程，在子线程进行文本的转化
    m_webview->setContent(html.toUtf8());
}

void QLiteNoteWindow::WebBlack()
{
    m_webview->setContent("<html><body> </body></html>");
}

void QLiteNoteWindow::ShowNote(const QString &path)
{
    QFile file(path);
    
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream *text = new QTextStream(&file);
        text->setCodec("UTF-8");
        QString mk = text->readAll();

        //QString html = ConvertToMarkdown(mk);
        m_thread->InsertMarkdown(mk);
       
        delete text;
    } else {
        WebBlack();
    }
}

void QLiteNoteWindow::EditNote(const QString &path)
{
    //用qt的QDesktopServices不支持中文路径，就算转化成utf8后也不行
    //QFileInfo f(s);
    //printf("EditNote: %s\n", s.toLocal8Bit().data());
    //QDesktopServices::openUrl(QUrl(path.toUtf8()));

    QFileInfo f(path);

    if (f.isFile()) {
#if defined(Q_OS_WIN32)
        QString s = QDir::convertSeparators(path);
        ShellExecute(s.toLocal8Bit().data());
#endif 
    }
}

void QLiteNoteWindow::EditNote()
{
    if (m_now_item) {
        QString s = m_now_item->data(1, 0).toString();
        EditNote(s);
    }
}

void QLiteNoteWindow::OpenExplorer()
{
    if (m_now_item) {
        QString s = m_now_item->data(1, 0).toString();
        QFileInfo f(s);

#if defined(Q_OS_WIN32)

        QString path = tr("OpenInExplorer.exe ") +f.absoluteFilePath();
        path = QDir::convertSeparators(path);
        system(path.toLocal8Bit().data());

#elif defined(Q_OS_MAC)

        //TODO: Mac

#elif defined(Q_OS_LINUX) 

        //TODO: Linux
#endif
        //QString abs_path = f.absFilePath();
        //QString absolute_file = f.absoluteFilePath();
        //QString absolute = f.absolutePath();
        //QString dir_path = f.dirPath();
        //QString file_path = f.filePath();
        //QString path = f.path();
        //QString ss = f.absolutePath();
        //QUrl url = QUrl::fromEncoded(ss.toUtf8());
        //QString r = url.toString();
        //printf("url: %s\n", r.toLocal8Bit().data());
        //QDesktopServices::openUrl(url);
    }
}

void QLiteNoteWindow::NewRootDir()
{
    printf("NewRoot\n");
}

void QLiteNoteWindow::RefreshAll()
{
    RefreshRoot(m_note_path);
}



void QLiteNoteWindow::UpdateStatue()
{
    if (m_now_item) {

        QString s = m_now_item->data(1, 0).toString();
        QFileInfo f(s);

        if (f.isFile()) {
            m_path_label->setText(s);

            m_new_note_action->setEnabled(false);
            m_new_dir_action->setEnabled(false);

        } else if (f.isDir()) {
            m_path_label->setText(s);

            m_new_note_action->setEnabled(true);
            m_new_dir_action->setEnabled(true);
        }

    } else {
        m_new_note_action->setEnabled(false);
        m_new_dir_action->setEnabled(false);
    }
}

void QLiteNoteWindow::WriteSettings()
{
    QSettings settings("Software xiangism", "QtLiteNote");

    settings.setValue("geometry", saveGeometry());

    QList<int> ss = m_split->sizes();
    settings.setValue("split", ss[0]);
}

void QLiteNoteWindow::ReadSettings()
{
    QSettings settings("Software xiangism", "QtLiteNote");

    QVariant geometry = settings.value("geometry");
    restoreGeometry(geometry.toByteArray());

    int s = settings.value("split").toInt();
    QList<int> ss;
    ss.push_back(s==0 ? 150 : s);
    ss.push_back(100);
    m_split->setSizes(ss);
}

void QLiteNoteWindow::ShowAbout()
{
    QMessageBox::about(this, tr("About QtLiteNote"),
        QString::fromUtf8("<h2>QtLiteNote 1.0</h2>"
        "<p>CopyRight &copy; 2015 xiangism Inc."
        "<p>LiteNote是一款基于Markdown渲染,<p>"
        "   以简洁、实用为目的开源笔记软件~"
        "<p><a href=\"http://www.cnblogs.com/xiangism\">cnblogs xiangism</a>"
        ));
}
