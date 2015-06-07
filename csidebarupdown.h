#ifndef CSIDEBARUPDOWN_H
#define CSIDEBARUPDOWN_H

#include <QWidget>
class CEditorTabWidget;
class ISideBarUpDown;

class CSideBarUpDown : public QWidget
{
    Q_OBJECT
public:
    explicit CSideBarUpDown(QWidget *parent, ISideBarUpDown *pBarParent);

    int getPositionY();
    void setPositionY(int iPositionY);
    void setMinPositionY(int iMinPositionY);
signals:

public slots:

protected:
    virtual void mousePressEvent(QMouseEvent *e);
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void mouseReleaseEvent(QMouseEvent *e);
private:
    ISideBarUpDown *m_pParent;
    int m_iMinPositionY;
    int m_iPositionY;
    QPoint m_pointPressed;
    QPoint m_pointParentPressed;
    bool m_bMouseDown;
    int m_iPressedPositionY;

    void moveSideBar(QMouseEvent *e);
};

#endif // CSIDEBARUPDOWN_H
