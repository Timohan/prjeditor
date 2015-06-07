#ifndef CSIDEBAR_H
#define CSIDEBAR_H

#include <QWidget>
class CMainWindow;
class ISideBar;

class CSideBar : public QWidget
{
    Q_OBJECT
public:
    explicit CSideBar(CMainWindow *parent, ISideBar *pBarParent);
    
    int getPositionX();
signals:
    
public slots:
    
protected:
    virtual void mousePressEvent(QMouseEvent *e);
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void mouseReleaseEvent(QMouseEvent *e);
private:
    ISideBar *m_pSideBar;
    CMainWindow *m_pMainWindow;
    int m_iPositionX;
    QPoint m_pointPressed;
    QPoint m_pointParentPressed;
    bool m_bMouseDown;
    int m_iPressedPositionX;

    void moveSideBar(QMouseEvent *e);
};

#endif // CSIDEBAR_H
