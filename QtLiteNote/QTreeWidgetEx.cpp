#include "QTreeWidgetEx.h"
#include <QtGUI>

QTreeWidgetEx::QTreeWidgetEx(QWidget *par)
: QTreeWidget(par),
m_now_select_node(NULL),
m_pre_click_time(0),
m_start_edit(false)
{
    connect(this, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(UpdateEdit(QTreeWidgetItem*)));
    setSelectionMode(SingleSelection);
    m_time.start();
}

QTreeWidgetEx::~QTreeWidgetEx(void)
{
}

void QTreeWidgetEx::SelectAboveOrBelow(bool is_above)
{
   if (m_now_select_node) {
       QTreeWidgetItem *now = NULL;
       if (is_above) {
           now = itemAbove(m_now_select_node);
       } else {
           now = itemBelow(m_now_select_node);
       }
       
       if (now) {
           SetSelectItem(now);
       }

   } else {
        if (this->topLevelItemCount() != 0) {
            QTreeWidgetItem *now = this->topLevelItem(0);
            if (now) {
                SetSelectItem(now);
            }
        }
   }
   
}

void QTreeWidgetEx::mousePressEvent(QMouseEvent *event)
{
    QTreeWidgetItem *item = this->itemAt(event->x(), event->y());

    if (!item) {
        if (event->button() == Qt::RightButton) {
            emit rightClick(NULL);
        }
        return;
    }
    
    if (event->button() == Qt::LeftButton) {

        //通过单击来改变节点的收展情况
        if (item->childCount() != 0) {
            if (item->isExpanded()) {
                item->setExpanded(false);
            } else {
                item->setExpanded(true);
            }
        }
        int now = m_time.elapsed();
        if (m_now_select_node == item) {
            if (now - m_pre_click_time < 500) {
                emit doubleClick(item);
            }
        }
        m_pre_click_time = now;

        SetSelectItem(item);

    } else if (event->button() == Qt::RightButton) {
        SetSelectItem(item);
        emit rightClick(item);
    }

}

void QTreeWidgetEx::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
        case Qt::Key_Up:
            SelectAboveOrBelow(true);
            break;

        case Qt::Key_Down:
            SelectAboveOrBelow(false);
            break;

        case Qt::Key_Return:
        case Qt::Key_Enter:
            if (m_start_edit) {
                QTreeWidget::keyPressEvent(event);
                break;
            }

        case Qt::Key_Space:
            if (m_now_select_node && m_now_select_node->childCount() != 0) {
                if (m_now_select_node->isExpanded()) {
                    m_now_select_node->setExpanded(false);
                } else {
                    m_now_select_node->setExpanded(true);
                }
            }
            break;

        case Qt::Key_F2:
            if (m_now_select_node) {
                this->editItem(m_now_select_node, 0);
                m_start_edit = true;
            }
            break;

        case Qt::Key_F5:
            {
                printf("Tree_f5\n");
            }
            break;
            
        case Qt::Key_Delete:
            {
                if (m_now_select_node) {
                    emit itemDelete(m_now_select_node);
                }
                //printf("Tree_Delete\n");
            }
            break;
    }
}

void QTreeWidgetEx::UpdateEdit(QTreeWidgetItem *item)
{
    if (item && m_start_edit) {
        m_start_edit = false;
        emit itemEdited(item);
    }
}

void QTreeWidgetEx::SetSelectItem(QTreeWidgetItem *item)
{
    if (item != m_now_select_node) {
        if (m_now_select_node) {
            m_now_select_node->setSelected(false);
        }
        if (item) {
            item->setSelected(true);
            m_now_select_node = item;
            scrollToItem(item);
        } else {
            m_now_select_node = NULL;
        }

        emit itemSelect(m_now_select_node);
    }
}
