#pragma once

#include "QtHead.h"

class QTreeView;
class QTreeWidget;
class QSplitter;
class QLabel;
class QTreeWidgetEx;
class QTreeWidgetItem;
class QTextEdit;
class QWebView;
class MarkdownThread;
class AnchorNode;

class QLiteNoteWindow : public QMainWindow
{
    Q_OBJECT
public:
    QLiteNoteWindow(QString path, QWidget *parent = 0);
    ~QLiteNoteWindow(void);

protected:
    void closeEvent(QCloseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

private:
    void CreateAction();
    void CreateMenu();
    void CreateStatus();
    void CreateMkLevelTree();

    void RefreshRoot(const QString &path);
    void RefreshNode(QTreeWidgetItem *item, bool scan_child_dir = false);

    //重载刷新某个节点的父节点，用于对节点删除或重命名后
    //不应该笼统地用这个函数，而应该分开，重命名后不需要刷新整个父节点
    //void ReNodeParent(QTreeWidgetItem *item);

    //找到一个没用过的新文件名，如果是找文件夹名则post为空，如果是文本文件则为.txt
    QString FindNewFileName(const QString &path, const QString &post);
    //展开现有的节点并且选中新建的那个节点  
    void ExpandAndSelectNew(const QString &path);
    void ClearNode(QTreeWidgetItem *node);

    void UpdateStatue();
    void EditNote(const QString &path);

    void WriteSettings();
    void ReadSettings();

    void ShowNote();
    void WebBlack();

	void SetMkLevel(AnchorNode *node);
	void SetChLevel(QTreeWidgetItem *item, AnchorNode *node);

private slots:
    void TreeItemSelect(QTreeWidgetItem *item);
    void TreeItemDelete(QTreeWidgetItem *item);
    void TreeItemRename(QTreeWidgetItem *item);
    void TreeRightClick(QTreeWidgetItem *item);
    void TreeItemDoubleClick(QTreeWidgetItem *item);
    void TreeItemExpand(QTreeWidgetItem *item);
    void TreeItemCollapsed(QTreeWidgetItem *item);
    void TreeItemKeyItem(QTreeWidgetItem *item);

    void MarkLevelItemSelect(QTreeWidgetItem *item);

    void ConvertEnd(const QString &html, void *anchorNode);

    void AddNewNote();
    void AddNewDir();
    void DeleteItem();
    void EditNote();
    void RenameItem();
    void OpenExplorer();
    void RefreshAll();
    void NewRootDir();
    void ShowTreeCheck();
    void ShowMkLevekCheck();
    void ResumeTrash();

    void ShowAbout();

private:
    QString m_note_path;
    QString m_trash_path;
    QSplitter *m_split;

    QTreeWidgetEx *m_note_tree;
    QTextEdit *m_edit_box;
    QWebView *m_webview;
    QTreeWidgetEx *m_mkLevel_tree;

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
    QAction *m_delete_action;
    QAction *m_edit_action;
    QAction *m_open_explorer_action;
    QAction *m_new_root_action;
    QAction *m_refresh_action;
    QAction *m_show_tree_action;
    QAction *m_show_mkLevel_action;
    QAction *m_resume_trash_action;
    QAction *m_rename_action;

    QLabel *m_path_label;

    QMenu *m_file_menu;
    QMenu *m_edit_menu;
    QMenu *m_options_menu;
    QMenu *m_help_menu;

    QAction *m_exit_action;
    QAction *m_about_action;

    MarkdownThread *m_thread;
    
    bool m_is_control_down;
};
