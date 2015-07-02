#pragma once
#include "QMainWindow"

class QTreeView;
class QTreeWidget;
class QSplitter;
class QLabel;
class QTreeWidgetEx;
class QTreeWidgetItem;
class QTextEdit;
class QWebView;
class MarkdownThread;

class QLiteNoteWindow : public QMainWindow
{
    Q_OBJECT
public:
    QLiteNoteWindow(QWidget *parent = 0);
    ~QLiteNoteWindow(void);

protected:
    void closeEvent(QCloseEvent *event);
    void keyPressEvent(QKeyEvent *event);

private:
    void CreateAction();
    void CreateMenu();
    void CreateStatus();

    void RefreshRoot(const QString &path);
    void RefreshNode(QTreeWidgetItem *item, const QString &path, bool scan_child_dir = false);

    //�ҵ�һ��û�ù������ļ�������������ļ�������postΪ�գ�������ı��ļ���Ϊ.txt
    QString FindNewFileName(const QString &path, const QString &post);
    //չ�����еĽڵ㲢��ѡ���½����Ǹ��ڵ�
    void ExpandAndSelectNew(const QString &path);
    void ClearNode(QTreeWidgetItem *node);

    void UpdateStatue();
    void EditNote(const QString &path);

    void WriteSettings();
    void ReadSettings();

    void ShowNote();
    void WebBlack();

private slots:
    void TreeItemSelect(QTreeWidgetItem *item);
    void TreeItemDelete(QTreeWidgetItem *item);
    void TreeItemEdited(QTreeWidgetItem *item);
    void TreeRightClick(QTreeWidgetItem *item);
    void TreeItemDoubleClick(QTreeWidgetItem *item);
    void TreeItemExpand(QTreeWidgetItem *item);
    void TreeItemCollapsed(QTreeWidgetItem *item);

    void ConvertEnd(const QString &html);

    void AddNewNote();
    void AddNewDir();
    void EditNote();
    void OpenExplorer();
    void RefreshAll();
    void NewRootDir();

    void ShowAbout();

private:
    QString m_note_path;
    QString m_trash_path;
    QSplitter *m_split;

    QTreeWidgetEx *m_tree;
    QTextEdit *m_edit_box;
    QWebView *m_webview;

    QTreeWidgetItem *m_now_item;

    QFont m_tree_font;

    QIcon m_dir_icon;
    QIcon m_dir_open_icon;
    QIcon m_note_icon;
    QMenu *m_dir_menu;
    QMenu *m_note_menu;
    QMenu *m_black_menu;

    QAction *m_new_note_action;
    QAction *m_new_dir_action;
    QAction *m_edit_action;
    QAction *m_open_explorer_action;
    QAction *m_new_root_action;
    QAction *m_refresh_action;

    QLabel *m_path_label;

    QMenu *m_file_menu;
    QMenu *m_edit_menu;
    QMenu *m_options_menu;
    QMenu *m_help_menu;

    QAction *m_exit_action;
    QAction *m_about_action;

    MarkdownThread *m_thread;
};
