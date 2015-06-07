#ifndef CEDITORTABWIDGET_H
#define CEDITORTABWIDGET_H

#include <QWidget>
#include <QList>
#include "isidebarupdown.h"
class CButton;
class CEditorTabBar;
class CTextEditStyle;
class QStackedWidget;
class CMainWindow;
class CEditorWidget;
class CDialogFind;
class CFindFromFilesTreeView;
class CSideBarUpDown;

class CEditorTabWidget : public QWidget, ISideBarUpDown
{
    Q_OBJECT
public:
    explicit CEditorTabWidget(CMainWindow *parent);
    ~CEditorTabWidget();

    /**
     * @brief addNewTab
     * @param strFileName
     *   file name of the tab, "" if new "untitled" tab
     */
    void addNewTab();

    /**
     * @brief setWidgetsForFolder
     * Set Widgets for this folder path
     * @param strFolderPath
     */
    void setWidgetsForFolder(QString strFolderPath, int iTabOpenIndex);

    static QString getTabNameFromFileName(const QString &strFileName);

    void textChanged(qint64 iId, bool bTextIsSame);

    void setTextEditStyle(CTextEditStyle *style);

    void searchFromFiles();

    void setCursorPosition(int iLineNumber, QString strToText);

    void onFileRename(QString strFrom, QString strTo);

    virtual void refreshPosition();

    /**
     * @brief getSelectedText
     * get selected text from current open text editor
     * @return selected text, or "" if not found
     */
    QString getSelectedText() const;
signals:

public slots:
    void tabBarCurrentChanged(int index);
    void onOpenFile(QString strFileName);

    void save();
    void saveAs();
    void saveAll();

    /**
     * @brief searchNext
     * called when F3 (Search Next is clicked)
     */
    void searchNext(bool bNewSearch = false);

    /**
     * @brief searchNext
     * called when Shift-F3 (Search Previous is clicked)
     */
    void searchPrevious();

    void replace();

    bool onClose();

public slots:
    void onTabCloseRequested(int index);
    void onCloseFindFromFilesClicked();

protected:
    virtual void resizeEvent(QResizeEvent *e);
    
private:
    bool m_bSideBarPositionDone;
    CSideBarUpDown *m_pSideBarUpDown;
    CEditorTabBar *m_pEditorTabBar;
    QStackedWidget *m_pStackedEditorWidgets;

    QList<CEditorWidget *>m_listEditorWidget;

    CFindFromFilesTreeView *m_pFindFromFilesTreeView;
    CButton *m_buttonCloseFindFromFiles;

    /**
     * @brief m_pMainWindow
     * pointer to main window
     */
    CMainWindow *m_pMainWindow;

    void setProjectListForFileName(QString strFileName);

    void setVisibleById(qint64 id, bool bChangeStyle = true);

    void setEditorWidgetTabColor(qint64 iId);

    void removeWidget(int iIndex, bool bRemoveFromEditorList);
};

#endif // CEDITORTABWIDGET_H
