#include <QMouseEvent>
#include <QDebug>
#include "csidebar.h"
#include "cmainwindow.h"
#include "isidebar.h"

CSideBar::CSideBar(CMainWindow *parent, ISideBar *pSideBar) :
    QWidget(parent),
    m_pSideBar(pSideBar),
    m_pMainWindow(parent),
    m_iPositionX(200),
    m_bMouseDown(false)
{
    setCursor(Qt::SizeHorCursor);
}

void CSideBar::mousePressEvent(QMouseEvent *e)
{
    m_iPressedPositionX = m_iPositionX;
    m_bMouseDown = true;
    m_pointPressed = e->pos();
    m_pointParentPressed = mapToParent(m_pointPressed);
}

void CSideBar::moveSideBar(QMouseEvent *e)
{
    QPoint pointPressed = e->pos();
    QPoint pointParentPressed = mapToParent(pointPressed);

    m_iPositionX = pointParentPressed.x() - m_pointParentPressed.x() + m_iPressedPositionX;

    if (!m_pSideBar) {

        m_pSideBar->refreshPosition();
    }
    else {

        m_pMainWindow->refreshPosition();
    }
}

int CSideBar::getPositionX()
{
    return m_iPositionX;
}



void CSideBar::mouseMoveEvent(QMouseEvent *e)
{
    if (m_bMouseDown)
    {
        moveSideBar(e);
    }
}

void CSideBar::mouseReleaseEvent(QMouseEvent *e)
{
    m_bMouseDown = false;
    moveSideBar(e);
}
