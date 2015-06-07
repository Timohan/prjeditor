#include <QMouseEvent>
#include <QDebug>
#include "csidebarupdown.h"
#include "ceditortabwidget.h"
#include "isidebarupdown.h"

CSideBarUpDown::CSideBarUpDown(QWidget *parent, ISideBarUpDown *pBarParent) :
    QWidget(parent),
    m_pParent(pBarParent),
    m_iMinPositionY(40),
    m_iPositionY(200),
    m_bMouseDown(false)
{
    setCursor(Qt::SizeVerCursor);
}

void CSideBarUpDown::mousePressEvent(QMouseEvent *e)
{
    m_iPressedPositionY = m_iPositionY;
    m_bMouseDown = true;
    m_pointPressed = e->pos();
    m_pointParentPressed = mapToParent(m_pointPressed);
}

void CSideBarUpDown::moveSideBar(QMouseEvent *e)
{
    QPoint pointPressed = e->pos();
    QPoint pointParentPressed = mapToParent(pointPressed);
    int m_iPrevPositionY = m_iPositionY;
    m_iPositionY = pointParentPressed.y() - m_pointParentPressed.y() + m_iPressedPositionY;
    if (m_iMinPositionY > m_iPositionY) {
        m_iPositionY = m_iMinPositionY;
    }

    if (m_iPrevPositionY != m_iPositionY) {

        m_pParent->refreshPosition();
    }
}

int CSideBarUpDown::getPositionY()
{
    return m_iPositionY;
}

void CSideBarUpDown::setPositionY(int iPositionY)
{
    m_iPositionY = iPositionY;
}

void CSideBarUpDown::setMinPositionY(int iMinPositionY)
{
    m_iMinPositionY = iMinPositionY;

    if (m_iMinPositionY > m_iPositionY) {

        m_iPositionY = m_iMinPositionY;
        m_pParent->refreshPosition();
    }
}

void CSideBarUpDown::mouseMoveEvent(QMouseEvent *e)
{
    if (m_bMouseDown)
    {
        moveSideBar(e);
    }
}

void CSideBarUpDown::mouseReleaseEvent(QMouseEvent *e)
{
    m_bMouseDown = false;
    moveSideBar(e);
}
