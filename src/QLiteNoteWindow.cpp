#include "QLiteNoteWindow.h"
#include "QTreeWidgetEx.h"
#include "MarkdownThread.h"
#include "MkNode.h"
#include "QFileEx.h"

#if defined(Q_OS_WIN32)
#include <windows.h>
#include "utility.h"

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

QTreeWidgetItem *FindChildWithText(QTreeWidgetItem *item, const QString &text)
{
	for (int i = 0; i < item->childCount(); ++i) {
		QTreeWidgetItem *ch = item->child(i);
		QString t = ch->text(0);
		//qDebug() << "FindItem: " << t;
		if (t == text) {
			return ch;
		}
	}
	return NULL;
}

/// sdfasdfasd
//////////////////////////////////////////////////////////////////////////
QLiteNoteWindow::QLiteNoteWindow(QString path, QWidget *parent)
    : QMainWindow(parent),
    m_split(NULL),
    m_note_tree(NULL),
    m_edit_box(NULL),
    m_webview(NULL),
    m_mkLevel_tree(NULL),
    m_now_item(NULL),
    m_tree_font(QString::fromUtf8("微软雅黑"), 10),
    m_dir_icon(":/ras/dir.png"),
    m_dir_open_icon(":/ras/dir_open.png"),
    m_note_icon(":/ras/note.png"),
    m_dir_menu(NULL),
    m_note_menu(NULL),
    m_black_menu(NULL),
    m_new_note_action(NULL),
    m_new_dir_action(NULL),
    m_delete_action(NULL),
    m_edit_action(NULL),
    m_open_explorer_action(NULL),
    m_refresh_action(NULL),
    m_show_tree_action(NULL),
    m_show_mkLevel_action(NULL),
    m_resume_trash_action(NULL),
    m_rename_action(NULL),
    m_new_root_action(NULL),
    m_path_label(NULL),
    m_file_menu(NULL),
    m_edit_menu(NULL),
    m_options_menu(NULL),
    m_help_menu(NULL),
    m_exit_action(NULL),
    m_about_action(NULL),
    m_thread(NULL),
    m_is_control_down(false),
	m_is_refreshNote(false)
{
    CreateAction();
    CreateMenu();
    CreateStatus();

    m_note_tree = new QTreeWidgetEx(this);
    m_note_tree->setHeaderHidden(true);
    m_note_tree->setRootIsDecorated(true);
    m_note_tree->setVerticalScrollMode(QTreeWidget::ScrollPerPixel);
    //m_tree->setFont(font);
    //m_tree->setStyleSheet("微软雅黑");

    connect(m_note_tree, SIGNAL(itemSelect(QTreeWidgetItem*)), this, SLOT(TreeItemSelect(QTreeWidgetItem*)));
    connect(m_note_tree, SIGNAL(itemDelete(QTreeWidgetItem*)), this, SLOT(TreeItemDelete(QTreeWidgetItem*)));
    connect(m_note_tree, SIGNAL(itemRename(QTreeWidgetItem*)), this, SLOT(TreeItemRename(QTreeWidgetItem*)));
    connect(m_note_tree, SIGNAL(rightClick(QTreeWidgetItem*)), this, SLOT(TreeRightClick(QTreeWidgetItem*)));
    connect(m_note_tree, SIGNAL(doubleClick(QTreeWidgetItem*)), this, SLOT(TreeItemDoubleClick(QTreeWidgetItem*)));
    connect(m_note_tree, SIGNAL(itemExpanded(QTreeWidgetItem *)), this, SLOT(TreeItemExpand(QTreeWidgetItem *)));
    connect(m_note_tree, SIGNAL(itemCollapsed(QTreeWidgetItem*)), this, SLOT(TreeItemCollapsed(QTreeWidgetItem*)));
    connect(m_note_tree, SIGNAL(spaceKeyItem(QTreeWidgetItem*)), this, SLOT(TreeItemKeyItem(QTreeWidgetItem*)));

    m_webview = new QWebView(this);
    //m_webview = new QWebEngineView(this);

    CreateMkLevelTree();

    m_split = new QSplitter(Qt::Horizontal);
    m_split->addWidget(m_note_tree);
    m_split->addWidget(m_webview);
    m_split->addWidget(m_mkLevel_tree);
    m_split->setStretchFactor(1, 1);

    m_thread = new MarkdownThread;
    connect(m_thread, SIGNAL(ConvertEnd(const QString &, void *)), this, SLOT(ConvertEnd(const QString&, void*)));
    m_thread->start();

    ReadSettings();
    UpdateStatue();

    setWindowIcon(QIcon(":/ras/app.png"));
    setCentralWidget(m_split);

    RefreshRoot(path);
    WebBlack();

    m_note_root_path = path;
    m_trash_path = m_note_root_path + QDir::separator() + "trash";
    QDir dir(path);
    dir.mkdir("trash");
}

QLiteNoteWindow::~QLiteNoteWindow(void)
{
}

void QLiteNoteWindow::closeEvent(QCloseEvent *event)
{
	delete m_thread;
    WriteSettings();
}

void QLiteNoteWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
        case Qt::Key_Control:
            m_is_control_down = true;
            break;
            
		case Qt::Key_F5:
			RefreshNowNote();
			break;
            
        case Qt::Key_R:
			RefreshNowNote();
            break;
            
#if defined(Q_OS_MAC)
        case Qt::Key_W:
            if (m_is_control_down) {
                showMinimized();
            }
            break;
#endif

        case Qt::Key_Delete:
            printf("Win_Delete\n");
            break;
            
        case Qt::Key_Meta:
            printf("meta\n");
            break;
    }
  
}
void QLiteNoteWindow::keyReleaseEvent(QKeyEvent *event) 
{
    switch (event->key()) {
        case Qt::Key_Control:
            m_is_control_down = false;
            break;
    }
}

void QLiteNoteWindow::CreateAction()
{
    //
    m_exit_action = new QAction(tr("E&xit"), this);
    connect(m_exit_action, SIGNAL(triggered()), this, SLOT(close()));

    m_about_action = new QAction(QString::fromUtf8("关于"), this);
    m_about_action->setIcon(QIcon(":/ras/app.png"));
    connect(m_about_action, SIGNAL(triggered()), this, SLOT(ShowAbout()));

    //
    m_new_note_action = new QAction(tr("NewNote"), this);
    m_new_note_action->setIcon(m_note_icon);
    connect(m_new_note_action, SIGNAL(triggered()), this, SLOT(AddNewNote()));

    m_new_dir_action = new QAction(tr("NewDir"), this);
    m_new_dir_action->setIcon(m_dir_icon);
    connect(m_new_dir_action, SIGNAL(triggered()), this, SLOT(AddNewDir()));

    m_delete_action = new QAction(tr("Delete"), this);
    m_delete_action->setIcon(QIcon(":/ras/delete.png"));
    connect(m_delete_action, SIGNAL(triggered()), this, SLOT(DeleteItem()));

    m_edit_action = new QAction(tr("EditNote"), this);
    m_edit_action->setIcon(QIcon(":/ras/edit.png"));
    connect(m_edit_action, SIGNAL(triggered()), this, SLOT(EditNote()));

    m_open_explorer_action = new QAction(tr("open explorer"), this);
    m_open_explorer_action->setIcon(QIcon(":/ras/computer.png"));
    connect(m_open_explorer_action, SIGNAL(triggered()), this, SLOT(OpenExplorer()));

    m_new_root_action = new QAction(QString::fromUtf8("添加目录"), this);
    m_new_root_action->setIcon(m_dir_icon);
    connect(m_new_root_action, SIGNAL(triggered()), this, SLOT(NewRootDir()));

    m_refresh_action = new QAction(QString::fromUtf8("全部刷新"), this);
    m_refresh_action->setIcon(QIcon(":/ras/refresh.png"));
    connect(m_refresh_action, SIGNAL(triggered()), this, SLOT(RefreshAll()));

    m_show_tree_action = new QAction(QString::fromUtf8("显示目录导航"), this);
    m_show_tree_action->setCheckable(true);
    m_show_tree_action->setChecked(true);
    connect(m_show_tree_action, SIGNAL(triggered()), this, SLOT(ShowTreeCheck()));

    m_show_mkLevel_action = new QAction(QString::fromUtf8("显示内容结构"), this);
    m_show_mkLevel_action->setCheckable(true);
    m_show_mkLevel_action->setChecked(true);
    connect(m_show_mkLevel_action, SIGNAL(triggered()), this, SLOT(ShowMkLevekCheck()));

    m_resume_trash_action = new QAction(QString::fromUtf8("恢复删除文件"), this);
    connect(m_resume_trash_action, SIGNAL(triggered()), this, SLOT(ResumeTrash()));

    m_rename_action = new QAction(QString::fromUtf8("改名"), this);
    m_rename_action->setIcon(QIcon(":/ras/rename.png"));
    connect(m_rename_action, SIGNAL(triggered()), this, SLOT(RenameItem()));
}

void QLiteNoteWindow::CreateMenu()
{
    //top menu
    m_file_menu = menuBar()->addMenu(tr("&File"));
    m_file_menu->addAction(m_new_note_action);
    m_file_menu->addAction(m_new_dir_action);
    m_file_menu->addSeparator();
    m_file_menu->addAction(m_delete_action);
    m_file_menu->addSeparator();
    m_file_menu->addAction(m_exit_action);

    m_edit_menu = menuBar()->addMenu(tr("&Edit"));
    m_edit_menu->addAction(m_edit_action);
    m_edit_menu->addAction(m_rename_action);
    m_edit_menu->addAction(m_delete_action);

    m_options_menu = menuBar()->addMenu(tr("&Options"));
    m_options_menu->addAction(m_show_tree_action);
    m_options_menu->addAction(m_show_mkLevel_action);

    m_help_menu = menuBar()->addMenu(tr("&Help"));
    m_help_menu->addAction(m_about_action);

    //context menu
    m_dir_menu = new QMenu(this);
    m_dir_menu->addAction(m_new_note_action);
    m_dir_menu->addAction(m_new_dir_action);
    m_dir_menu->addSeparator();
    m_dir_menu->addAction(m_rename_action);
    m_dir_menu->addAction(m_delete_action);
    m_dir_menu->addSeparator();
    m_dir_menu->addAction(m_open_explorer_action);

    m_note_menu = new QMenu(this);
    m_note_menu->addAction(m_edit_action);
    m_note_menu->addAction(m_rename_action);
    m_note_menu->addAction(m_delete_action);
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

void QLiteNoteWindow::CreateMkLevelTree()
{
    m_mkLevel_tree = new QTreeWidget(this);
    m_mkLevel_tree->setSelectionMode(QAbstractItemView::SingleSelection);
    m_mkLevel_tree->setHeaderHidden(true);
    m_mkLevel_tree->setRootIsDecorated(true);
    m_mkLevel_tree->setVerticalScrollMode(QTreeWidget::ScrollPerPixel);

	connect(m_mkLevel_tree, SIGNAL(itemPressed(QTreeWidgetItem *, int )), this, SLOT(MarkLevelItemSelect2(QTreeWidgetItem*,int)));
}

void QLiteNoteWindow::RefreshRoot(const QString &path)
{
    QDir dir(path);
    QStringList filters;
    QStringList fs = dir.entryList(filters, QDir::Dirs);

    int c = m_note_tree->topLevelItemCount();
    for (int i = 0; i < c; ++i) {
        QTreeWidgetItem *item = m_note_tree->topLevelItem(i);
        ClearNode(item);
    }
    m_note_tree->clear();
    m_note_tree->SetNowItemOnly(NULL);
    m_now_item = NULL;

    for (int i = 2; i < fs.size(); ++i) {
        QString s = fs.at(i);
        if (s.toLower() == "trash" || s.endsWith(".app")) {
            continue;
        }
		if (s == "platforms") {
			continue;
		}
        QString full_path = path + QDir::separator() + s;
        QStringList name;
        name.push_back(s);

        QTreeWidgetItem *d = new QTreeWidgetItem(name);
        d->setIcon(0, QIcon(":/ras/dir.png"));
        d->setData(1, Qt::UserRole, full_path);
        d->setFont(0, m_tree_font);
        d->setFlags(Qt::ItemIsEditable|Qt::ItemIsSelectable|Qt::ItemIsEnabled);

        m_note_tree->insertTopLevelItem(m_note_tree->topLevelItemCount(), d);

        RefreshNode(d);
    }

    if (m_note_tree->topLevelItemCount() != 0) {
        m_note_tree->SetSelectItem(m_note_tree->topLevelItem(0));
    }
}

void QLiteNoteWindow::RefreshNode(QTreeWidgetItem *item, bool scan_child_dir)
{
    QString path = item->data(1, Qt::UserRole).toString();

    ClearNode(item);

    QDir dir(path);
    QStringList dirs = dir.entryList(QDir::Dirs);

	QStringList txt_filters;
	txt_filters.push_back("*.txt");
	QStringList files = dir.entryList(txt_filters, QDir::Files);

    for (int i = 2; i < dirs.size(); ++i) {
        QString &dir = dirs[i];
      
		QString ss = path + QDir::separator() + dir;
		QStringList name;
		name.push_back(dir);

		QTreeWidgetItem *d = new QTreeWidgetItem(name);
		d->setIcon(0, m_dir_icon);
		d->setData(1, Qt::UserRole, ss);
        d->setFont(0, m_tree_font);
        d->setFlags(Qt::ItemIsEditable|Qt::ItemIsSelectable|Qt::ItemIsEnabled);

        item->insertChild(item->childCount(), d);

        if (scan_child_dir) {
            RefreshNode(d, false);
        }
    }

    for (int i = 0; i < files.size(); ++i) {
        QStringList name;
        QString s = files[i];
        int index = s.lastIndexOf(".");
        s = s.mid(0, index);

        name.push_back(s);

        QTreeWidgetItem *d = new QTreeWidgetItem(name);
        d->setIcon(0, m_note_icon);
        d->setFont(0, m_tree_font);
		d->setData(1, Qt::UserRole, path + QDir::separator() + files[i]);
        d->setFlags(Qt::ItemIsEditable|Qt::ItemIsSelectable|Qt::ItemIsEnabled);

        item->insertChild(item->childCount(), d);
    }
}

void QLiteNoteWindow::RefreshNowNote()
{
	m_is_refreshNote = true;

    QWebPage *page = m_webview->page();
    QWebFrame *frame = page->mainFrame();
    m_web_scroll = frame->scrollPosition();

    //QWebEnginePage *page = m_webview->page();
    //m_web_scroll = page->scrollPosition();

	int r = m_now_note_path.indexOf(m_note_root_path);
	if (r == 0) {
		QString p = m_now_note_path.mid(m_note_root_path.length() + 1);
		QStringList ps = p.split(QRegExp("[\\\\\\/]"));
		NavigationTree(ps);
	}
	ShowNote(m_now_note_path);
}

void QLiteNoteWindow::NavigationTree(const QStringList &names)
{
	if (names.empty()) {
		return;
	}
	QString top = names[0];
	QTreeWidgetItem *par = NULL;

	for (int i = 0; i < m_note_tree->topLevelItemCount(); ++i) {
		QTreeWidgetItem *item = m_note_tree->topLevelItem(i);
		QString text = item->text(0);

		if (top == text) {
			par = item;
			item->setExpanded(true);
			break;
		}
	}
	if (!par) {
		return;
	}

	for (int i = 1; i < names.size()-1; ++i) {
		if (!par) {
			break;
		}
		QTreeWidgetItem *item = FindChildWithText(par, names[i]);
		if (item) {
			par = item;
			item->setExpanded(true);
		}
	}
	QString s = names[names.size() - 1];
	int index = s.lastIndexOf(".");

	QTreeWidgetItem *item = FindChildWithText(par, s.mid(0, index));
	if (item) {
		m_note_tree->SetSelectItem(item);
	}
	
}

void QLiteNoteWindow::TreeItemSelect(QTreeWidgetItem *item)
{
	if (item) {
		m_now_item = item;
		QString s = m_now_item->data(1, Qt::UserRole).toString();
		QFileInfo f(s);
		if (f.isFile()) {
			m_now_note_path = s;
			ShowNote(s);
		}
		UpdateStatue();
	}
}

void QLiteNoteWindow::TreeItemDelete(QTreeWidgetItem *item)
{
    int r = QMessageBox::warning(this, "QtLiteNote", QString::fromUtf8("确定要删除吗?"), QMessageBox::Yes|QMessageBox::No);

    if (r == QMessageBox::Yes) {
		QString src = item->data(1, Qt::UserRole).toString();
        QString name = src.mid(m_note_root_path.size()+1);
        QFileInfo file(src);

        if (file.isFile()) {
            QCopyFile(src, m_trash_path + QDir::separator() + name);
            QFile::remove(src);

        } else if (file.isDir()) {
            //TODO:删除文件夹
            QCopyDir(src, m_trash_path);
            QDeleteDir(src);
        }

        if (item->parent()) {
            //选择同级节点,如果没有同级节点则选择父节点
            QTreeWidgetItem *par = item->parent();
            int c = par->childCount();

            if (c > 1) {
                int index = par->indexOfChild(item);

                if (index == c-1) {
                    m_note_tree->SetSelectItem(par->child(c - 2));
                } else {
                    m_note_tree->SetSelectItem(par->child(index+1));
                }
            } else {
               m_note_tree->SetSelectItem(par);
            }
            par->removeChild(item);
            delete item;

        } else {
            int c = m_note_tree->topLevelItemCount();
            if (c > 1) {
                int index = m_note_tree->indexOfTopLevelItem(item);

                if (index == c-1) {
                    m_note_tree->SetSelectItem(m_note_tree->topLevelItem(c-2));
                } else {
                    m_note_tree->SetSelectItem(m_note_tree->topLevelItem(index+1));
                }
            }
            m_note_tree->removeItemWidget(item, 0);
            delete item;
        }

    } // end r==Yes
}

void QLiteNoteWindow::TreeItemRename(QTreeWidgetItem *item)
{
    if (item) {
        //QString txt = item->data(0, Qt::UserRole).toString();
        QString txt = item->text(0);
        //windows下不支持的路径字符 \ / : * ? " < > |
        //其它平台就暂时也不用这些字符
        QString name = txt.remove(QRegExp("[\\\\\\/:*?\"\\<\\>\\|]"));
        QString old_path = item->data(1, Qt::UserRole).toString();
        QFileInfo info(old_path);

        QString new_path;
        if (info.isFile()) {
            new_path = info.absolutePath() + QDir::separator() + name + ".txt";
            m_now_note_path = new_path;
        } else {
            new_path = info.absolutePath() + QDir::separator() + name;
        }

        QDir d("");
        if (!d.rename(old_path, new_path)) {
            return;
        }

		item->setData(1, Qt::UserRole, new_path);

        if (info.isDir()) {
           RefreshNode(item);
        }
    } // end if item
}

void QLiteNoteWindow::TreeRightClick(QTreeWidgetItem *item)
{
	if (item) {
		m_now_item = item;
        QString path = m_now_item->data(1, Qt::UserRole).toString();
        QFileInfo file(path);
		if (file.isFile()) {
            m_now_note_path = path;
			m_note_menu->exec(cursor().pos());
		} else if (file.isDir()) {
			m_dir_menu->exec(cursor().pos());
		}
		UpdateStatue();

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
    //if (item) {
        //QString s = item->data(1, 0).toString();
        //EditNote(s);
    //}
}

void QLiteNoteWindow::TreeItemCollapsed(QTreeWidgetItem *item)
{
    if (item) {
        item->setIcon(0, m_dir_icon);
    }
}

void QLiteNoteWindow::TreeItemKeyItem(QTreeWidgetItem *item)
{
    if (item) {
		QString path = item->data(1, Qt::UserRole).toString();
        QFileInfo f(path);

        if (f.isFile()) {
            EditNote(path);
        }
    }
}

void QLiteNoteWindow::MarkLevelItemSelect2(QTreeWidgetItem *item, int column)
{
	if (item) {
		QString anchor = item->data(0, Qt::UserRole).toString();

        QWebPage *page = m_webview->page();
        QWebFrame *frame = page->mainFrame();
        frame->scrollToAnchor(anchor);

        /*QWebEnginePage *page = m_webview->page();
		char str[1024];
		sprintf(str, "window.location.hash = \"%s\"", anchor.toUtf8().data());
        page->runJavaScript(str);*/
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
            for (int i = 0; i < m_note_tree->topLevelItemCount(); ++i) {
                QTreeWidgetItem *it = m_note_tree->topLevelItem(i);

                if (it != item) {
                    it->setExpanded(false);
                }
            }
        }
        
        RefreshNode(item, true);
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

        for (int i = 0; i < m_now_item->childCount(); ++i) {
            QTreeWidgetItem *item = m_now_item->child(i);
			QString s = item->data(1, Qt::UserRole).toString();

            if (s == path) {
                m_note_tree->SetSelectItem(item);
                m_note_tree->StartEdit(item);
                break;
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
		QString path = m_now_item->data(1, Qt::UserRole).toString();
        QFileInfo info(path);

        if (info.isDir()) {
            QString new_path = FindNewFileName(path, QString(".txt"));

#if defined(Q_OS_WIN32)
            std::string gbk = ln::UTF8ToGBK(new_path.toUtf8().data());
            FILE *file = fopen(gbk.c_str(), "wt");
            fclose(file);
#elif defined(Q_OS_MAC)
            FILE *file = fopen(new_path.toLocal8Bit().data(), "wt");
            fclose(file);
#endif
            ExpandAndSelectNew(new_path);
        }
      
    }
}

void QLiteNoteWindow::AddNewDir()
{
    if (m_now_item) {
		QString path = m_now_item->data(1, Qt::UserRole).toString();
        QFileInfo info(path);

        if (info.isDir()) {
            QString new_path = FindNewFileName(path, QString(""));
            QDir dir;
            dir.mkdir(new_path);

            ExpandAndSelectNew(new_path);
        }
    }

}

void QLiteNoteWindow::DeleteItem()
{
    if (m_now_item) {
        TreeItemDelete(m_now_item);
    }
}

void QLiteNoteWindow::ConvertEnd(const QString &html, void *anchorNode)
{
    m_webview->setContent(html.toUtf8());
    //m_webview->setHtml(html.toUtf8());

	if (m_is_refreshNote) {
        QWebPage *page = m_webview->page();
        QWebFrame *frame = page->mainFrame();
        frame->setScrollPosition(m_web_scroll);
        m_is_refreshNote = false;

        //QWebEnginePage *page = m_webview->page();
		//frame->setScrollPosition(m_web_scroll);
		//page->scrollPositionChanged(m_web_scroll);
		//page->runJavaScript(QStringLiteral("window.scrollTo(%1, %2); [window.scrollX, window.scrollY];").arg(m_web_scroll.rx()).arg(m_web_scroll.ry()), invoke(this, &QtWebEngineWebWidget::handleScroll));

        //char str[1024];
        //sprintf(str, "window.scrollTo(%f, %f);", m_web_scroll.rx(), m_web_scroll.ry());
        //page->runJavaScript(str);
	}

	AnchorNode *node = (AnchorNode*)anchorNode;
	SetMkLevel(node);
	ReleaseAnchorNode(node);

    //WriteMdToHtml(html, "d:\\temp.html");
    //QUrl u("file:///d:\\temp.html");
    //m_webview->setUrl(u);
}

void QLiteNoteWindow::SetMkLevel(AnchorNode *node)
{
	m_mkLevel_tree->clear();

	if (node->m_children.empty()) {
		QList<int> ss = m_split->sizes();
		ss[2] = 0;
		m_split->setSizes(ss);
		return;
	} else {
		if (m_show_mkLevel_action->isChecked()) {
			QList<int> ss = m_split->sizes();
			ss[2] = 150;
			m_split->setSizes(ss);
		}
	}
	for (int i = 0; i < node->m_children.size(); ++i) {
		AnchorNode *p = node->m_children[i];
		QStringList name;
		name.push_back(QString::fromUtf8(p->m_name.c_str()));
		QTreeWidgetItem *item = new QTreeWidgetItem(name);
		item->setData(0, Qt::UserRole, QString::fromUtf8(p->m_anchorID.c_str()));
		item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
		m_mkLevel_tree->insertTopLevelItem(m_mkLevel_tree->topLevelItemCount(), item);

		SetChLevel(item, p);
	}
	m_mkLevel_tree->expandAll();
}

void QLiteNoteWindow::SetChLevel(QTreeWidgetItem *item, AnchorNode *node)
{
	for (int i = 0; i < node->m_children.size(); ++i) {
		AnchorNode *p = node->m_children[i];

		QStringList name;
		name.push_back(QString::fromUtf8(p->m_name.c_str()));
		QTreeWidgetItem *ch = new QTreeWidgetItem(name);
		ch->setData(0, Qt::UserRole, QString::fromUtf8(p->m_anchorID.c_str()));
		ch->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

		item->insertChild(item->childCount(), ch);
		SetChLevel(ch, p);
	}
}

void QLiteNoteWindow::WebBlack()
{
    //m_webview->setContent("<html><body> </body></html>");
	m_webview->setHtml("<html><body> </body></html>");
}

void QLiteNoteWindow::ShowNote(const QString &path)
{
	QFileInfo f(path);

	if (f.isFile()) {
		QFile file(path);

		if (file.open(QIODevice::ReadOnly)) {
			QTextStream text(&file);
			text.setCodec("UTF-8");

			QDir dir = f.absoluteDir();
			QVector<QString> ss;
			ss.push_back(dir.path());
			while (!text.atEnd()) {
				ss.push_back(text.readLine());
			}
			m_thread->InsertMarkdown(ss);

		} else {
			WebBlack();
		}
	}
}

void QLiteNoteWindow::EditNote(const QString &path)
{
    QFileInfo f(path);

    if (f.isFile()) {
#if defined(WIN32)
        QString s = QDir::toNativeSeparators(path);
        std::string gbk = ln::UTF8ToGBK(s.toUtf8().data());
        
        ShellExecute(gbk.c_str());

#elif defined(Q_OS_LINUX)
        char str[1024];
        sprintf(str, "%s \"%s\"", "gvim  ", f.absoluteFilePath().toLocal8Bit().data());
        system(str);

#elif defined(Q_OS_MAC)
        QUrl u = QUrl::fromLocalFile(path);
        bool r = QDesktopServices::openUrl(u);
        printf("EditNote: %s\n", path.data());
        printf("result: %d\n", r);
#endif
    }
}

void QLiteNoteWindow::EditNote()
{
    if (m_now_item) {
		QString s = m_now_item->data(1, Qt::UserRole).toString();
        EditNote(s);
    }
}

void QLiteNoteWindow::RenameItem()
{
    if (m_now_item) {
        m_note_tree->StartEdit(m_now_item);
    }
}

void QLiteNoteWindow::OpenExplorer()
{
    if (m_now_item) {
		QString s = m_now_item->data(1, Qt::UserRole).toString();
        QFileInfo f(s);

#if defined(Q_OS_WIN32)
        QString path = tr("OpenInExplorer.exe ") +f.absoluteFilePath();
        path = QDir::toNativeSeparators(path);
        
        std::string gbk = ln::UTF8ToGBK(path.toUtf8().data());
        system(gbk.c_str());
        
#elif defined(Q_OS_LINUX) 
        char str[1024];
        sprintf(str, "%s \"%s\"", "nautilus", f.absoluteFilePath().toLocal8Bit().data());
        system(str);
        
#elif defined(Q_OS_MAC)
        QUrl url = QUrl::fromLocalFile(f.absolutePath());
        QDesktopServices::openUrl(url);
#endif
    }
}

void QLiteNoteWindow::NewRootDir()
{
   QString new_path = FindNewFileName(m_note_root_path, QString(""));
   QFileInfo file(new_path);
   QDir dir;
   dir.mkdir(new_path);

   QStringList name;
   name.push_back(file.fileName());
   QTreeWidgetItem *d = new QTreeWidgetItem(name);
   d->setIcon(0, QIcon(":/ras/dir.png"));
   d->setData(1, Qt::UserRole, new_path);
   d->setFont(0, m_tree_font);
   d->setFlags(Qt::ItemIsEditable|Qt::ItemIsSelectable|Qt::ItemIsEnabled);

   m_note_tree->insertTopLevelItem(m_note_tree->topLevelItemCount(), d);

   for (int i = 0; i < m_note_tree->topLevelItemCount(); ++i) {
       QTreeWidgetItem *item = m_note_tree->topLevelItem(i);
	   QString s = item->data(1, Qt::UserRole).toString();

       if (s == new_path) {
           m_note_tree->SetSelectItem(item);
           m_note_tree->StartEdit(item);
           break;
       }
   }
}

void QLiteNoteWindow::ShowTreeCheck()
{
    QList<int> ss = m_split->sizes();
    if (m_show_tree_action->isChecked()) {
        ss[0] = 150;
    }
    else {
        ss[0] = 0;
    }
    m_split->setSizes(ss);
}

void QLiteNoteWindow::ShowMkLevekCheck()
{
    QList<int> ss = m_split->sizes();
    if (m_show_mkLevel_action->isChecked()) {
        ss[2] = 150;
    } else {
        ss[2] = 0;
    }
    m_split->setSizes(ss);
}

void QLiteNoteWindow::ResumeTrash()
{
    //TODO:恢复删除文件
}

void QLiteNoteWindow::RefreshAll()
{
    RefreshRoot(m_note_root_path);
}

void QLiteNoteWindow::UpdateStatue()
{
    if (m_now_item) {

		QString s = m_now_item->data(1, Qt::UserRole).toString();
        QFileInfo f(s);

        if (f.isFile()) {
            m_path_label->setText(s);

            m_new_note_action->setEnabled(false);
            m_new_dir_action->setEnabled(false);

        } else if (f.isDir()) {
            //m_path_label->setText(s);

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

    QList<int> si = m_split->sizes();
	settings.setValue("left_split", si[0]);
	settings.setValue("right_split", si[2]);
}

void QLiteNoteWindow::ReadSettings()
{
    QSettings settings("Software xiangism", "QtLiteNote");

    QVariant geometry = settings.value("geometry");
    restoreGeometry(geometry.toByteArray());

	//QList<QVariant> ss = settings.value("splits").toList();
	QList<int> si;
	int left = settings.value("left_split").toInt();
	int right = settings.value("right_split").toInt();
	si.push_back(left);
	si.push_back(0);
	si.push_back(right);
	m_split->setSizes(si);
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
