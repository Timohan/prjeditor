#ifndef CEDITORTABBAR_H
#define CEDITORTABBAR_H

#include <QTabBar>

class CEditorTabBarLabelWidget;

class CEditorTabBar : public QTabBar
{
    Q_OBJECT
public:
    explicit CEditorTabBar(QWidget *parent = 0);
    void closeTab(int index);
    void closeTab(CEditorTabBarLabelWidget *pTabLabel);

signals:
    


protected:
    virtual void tabInserted(int index);
    
};

#endif // CEDITORTABBAR_H
