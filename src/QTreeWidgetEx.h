#pragma once
#include "QtHead.h"


class QTreeWidgetEx : public QTreeWidget
{
    Q_OBJECT
public:
    QTreeWidgetEx(QWidget *parent = 0);
    ~QTreeWidgetEx(void);

    virtual void mousePressEvent(QMouseEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);

    void SetSelectItem(QTreeWidgetItem *item);
    void SetNowItemOnly(QTreeWidgetItem *item);

    void StartEdit(QTreeWidgetItem *item);

protected slots:
    void UpdateEdit(QTreeWidgetItem *item);
    
private:
    void SelectAboveOrBelow(bool is_above);

signals:
    void itemSelect(QTreeWidgetItem *item);
    void itemDelete(QTreeWidgetItem *item);
    void itemRename(QTreeWidgetItem *item);
    void rightClick(QTreeWidgetItem *item);
    void doubleClick(QTreeWidgetItem *item);

    void spaceKeyItem(QTreeWidgetItem *item);

private:
    QTreeWidgetItem *m_now_select_node;
    QTime m_time;
    int m_pre_click_time;
    bool m_start_edit;
};
