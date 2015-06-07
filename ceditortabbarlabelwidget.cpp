#include "ceditortabbarlabelwidget.h"
#include <QLabel>
#include "cbutton.h"
#include "application.h"
#include "ceditortabbar.h"
#include <QDebug>
#include <QMenu>

CEditorTabBarLabelWidget::CEditorTabBarLabelWidget(CEditorTabBar *parent, QString strTabName, bool bTextIsSame) :
    QWidget(parent),
    m_bTextIsSame(false),
    m_pParent(parent),
    m_iId(-1),
    m_strTabName(strTabName)
{ 
    m_labelTitle = new QLabel(strTabName, this);

    this->setFixedHeight(13);

    QFontMetrics fontMetrics(m_labelTitle->font());
    int iTextWidth = fontMetrics.width(m_labelTitle->text());
    m_labelTitle->resize(iTextWidth, 12);

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)),
        this, SLOT(ShowContextMenu(const QPoint&)));

    setTextIsChanged(bTextIsSame);
}


void CEditorTabBarLabelWidget::setTabName(const QString strName)
{
    m_strTabName = strName;
    m_labelTitle->setText(strName);

    // this is just silly trick to force re-draw label text
    m_bTextIsSame = !m_bTextIsSame;
    setTextIsChanged(!m_bTextIsSame);
}

qint64 CEditorTabBarLabelWidget::getNewId()
{
    static qint64 i = 0;
    i++;
    return i;
}


qint64 CEditorTabBarLabelWidget::getId() const
{
    return m_iId;
}

void CEditorTabBarLabelWidget::setId(qint64 iId)
{
    m_iId = iId;
}


void CEditorTabBarLabelWidget::resizeEvent(QResizeEvent *e)
{

    QWidget::resizeEvent(e);

    QFontMetrics fontMetrics(m_labelTitle->font());
    int iTextHeight = fontMetrics.height();
    m_labelTitle->setGeometry(0, height()/2 - iTextHeight/2 - 1, width(), iTextHeight + 1);
}


void CEditorTabBarLabelWidget::setTextIsChanged(bool bTextIsSame)
{
    if (m_bTextIsSame != bTextIsSame)
    {
        QString strColor;
        if (bTextIsSame)
        {
            strColor = "black";
        }
        else
        {
            strColor = "red";
        }

        QString strTemp = QString("<font color='%1'>%2</font>").arg(strColor).arg(m_strTabName);

        m_labelTitle->setText(strTemp);
        m_bTextIsSame = bTextIsSame;
    }
}

void CEditorTabBarLabelWidget::ShowContextMenu(const QPoint&pos)
{
    QPoint globalPos = this->mapToGlobal(pos);
    QMenu myMenu;
    myMenu.addAction(STRING_PROJECT_CLOSE);
    myMenu.addAction(STRING_RENAME);

    QAction* selectedItem = myMenu.exec(globalPos);
    if (selectedItem)
    {
        // something was chosen, do stuff

        if (selectedItem->text() == STRING_PROJECT_CLOSE)
        {
            m_pParent->closeTab(this);
        }
        else if (selectedItem->text() == STRING_RENAME)
        {

        }
    }
    else
    {
            // nothing was chosen
    }
}

bool operator==(const CEditorTabBarLabelWidget& lhs, const CEditorTabBarLabelWidget& rhs)
{
    if (lhs.getId() == rhs.getId()) {
        return true;
    }
    else {
        return false;
    }
}
