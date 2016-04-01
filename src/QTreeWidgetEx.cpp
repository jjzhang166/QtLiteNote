#include "QTreeWidgetEx.h"

QTreeWidgetEx::QTreeWidgetEx(QWidget *par)
: QTreeWidget(par),
m_now_select_node(NULL),
m_pre_click_time(0),
m_start_edit(false)
{
    setSelectionMode(SingleSelection);

    connect(this, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(UpdateEdit(QTreeWidgetItem*)));
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

void QTreeWidgetEx::mouseReleaseEvent(QMouseEvent *event)
{
	/*QTreeWidgetItem *item = this->itemAt(event->x(), event->y());
	if (item) {
		SetSelectItem(item);
	}*/
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
            //这里没有break. return,enter将会继续到space中
        case Qt::Key_Space:
            if (m_now_select_node) {
                if (m_now_select_node->childCount() != 0) {
                    if (m_now_select_node->isExpanded()) {
                        m_now_select_node->setExpanded(false);
                    } else {
                        m_now_select_node->setExpanded(true);
                    }
                } else {
                    emit spaceKeyItem(m_now_select_node);
                }
            }
            break;

        case Qt::Key_F2:
            if (m_now_select_node) {
                StartEdit(m_now_select_node);
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
            }
            break;
    }
}

void QTreeWidgetEx::UpdateEdit(QTreeWidgetItem *item)
{
    if (item && m_start_edit) {
        m_start_edit = false;
        emit itemRename(item);
    }
}

void QTreeWidgetEx::SetSelectItem(QTreeWidgetItem *item)
{
	if (item == NULL) {
		if (m_now_select_node) {
			m_now_select_node->setSelected(false);
		}
		m_now_select_node = NULL;
		return;
	}

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

void QTreeWidgetEx::SetNowItemOnly(QTreeWidgetItem *item)
{
    m_now_select_node = item;
}

void QTreeWidgetEx::StartEdit(QTreeWidgetItem *item)
{
    this->editItem(item, 0);
    m_start_edit = true;
}
