#include "ceditortabbar.h"
#include "ceditortabbarlabelwidget.h"
#include <QLabel>
#include <QDebug>

CEditorTabBar::CEditorTabBar(QWidget *parent) :
    QTabBar(parent)
{

    this->setTabsClosable(true);
}

void CEditorTabBar::tabInserted(int index)
{

    QString strTabName = this->tabText(index);

    this->setTabText(index, "");

    CEditorTabBarLabelWidget *pEditorTabBarLabelWidget = new CEditorTabBarLabelWidget(this, strTabName, true);
    this->setTabButton(index, QTabBar::LeftSide, pEditorTabBarLabelWidget);

    this->setCurrentIndex(index);

}

void CEditorTabBar::closeTab(int index)
{

    emit tabCloseRequested(index);
}

void CEditorTabBar::closeTab(CEditorTabBarLabelWidget *pTabLabel)
{

    int i;
    for (i=0;i<count();i++)
    {
        CEditorTabBarLabelWidget *pWidget = (CEditorTabBarLabelWidget *)tabButton(i, QTabBar::LeftSide);
        if (pTabLabel == pWidget)
        {
            emit tabCloseRequested(i);
            break;
        }
    }
}

