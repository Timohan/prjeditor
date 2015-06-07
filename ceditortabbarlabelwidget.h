#ifndef CEDITORTABBARLABELWIDGET_H
#define CEDITORTABBARLABELWIDGET_H

#include <QWidget>
class CButton;
class QLabel;
class CEditorTabBar;

class CEditorTabBarLabelWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CEditorTabBarLabelWidget(CEditorTabBar *parent, QString strTabName, bool bTextIsSame);
    
    void setTabName(const QString strName);

    qint64 getId() const;
    void setId(qint64 iId);

    static qint64 getNewId();

    void setTextIsChanged(bool bTextIsSame);

signals:
    
public slots:
    void ShowContextMenu(const QPoint &pos);

protected:
    virtual void resizeEvent(QResizeEvent *e);
    
private:
    bool m_bTextIsSame;
    CEditorTabBar *m_pParent;
    QLabel *m_labelTitle;
    qint64 m_iId;
    QString m_strTabName;
};

#endif // CEDITORTABBARLABELWIDGET_H
