#ifndef CTOOLBARWIDGET_H
#define CTOOLBARWIDGET_H

#include <QWidget>
class CButton;
class QComboBox;
class CTextEditStyle;
class CMainWindow;

class CToolbarWidget : public QWidget
{
    Q_OBJECT
public:
    CToolbarWidget(CMainWindow *parent);
    ~CToolbarWidget();

    void addTextStyleName(QString strStyleName);
    void setComboboxTextStyle(QString strStyleName);
    void setSaveEnabled(bool bEnabled);
    void setSaveAllEnabled(bool bEnabled);

    int getDefaultHeight();
    int getTabulatorIndex();
    void setTabulatorIndex(int iIndex);
signals:
    void onButtonNewClicked();
    void openNewFile();

    void onSave();
    void onSaveAll();

public slots:
    void buttonNewClicked();
    void buttonOpenClicked();
    void buttonSaveClicked();
    void buttonSaveAllClicked();

    void textStyleCurrentIndexChanged(int);
    void textTabulatorIndexChanged(int);

protected:
    virtual void resizeEvent(QResizeEvent *e);

private:
    bool m_bAllowCurrentIndexChange;

    CButton *m_buttonNew;
    CButton *m_buttonOpen;
    CButton *m_buttonSave;
    CButton *m_buttonSaveAll;

    QComboBox *m_comboboxTextStyle;
    QComboBox *m_comboboxTabulator;

    /**
     * @brief m_pMainWindow
     * pointer to main window
     */
    CMainWindow *m_pMainWindow;

};

#endif // CTOOLBARWIDGET_H
