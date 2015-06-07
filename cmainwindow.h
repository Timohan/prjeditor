#ifndef CMAINWINDOW_H
#define CMAINWINDOW_H

#include <QMainWindow>
#include "isidebarupdown.h"

class CToolbarWidget;
class CProjectListWidget;
class CFolderTreeView;
class CEditorTabWidget;
class CTextEditStyleList;
class CDialogFind;
class CSideBar;
class CSideBarUpDown;
class CDialogAbout;

namespace Ui {
class CMainWindow;
}

class CMainWindow : public QMainWindow, ISideBarUpDown
{
    Q_OBJECT
    
public:
    explicit CMainWindow(QWidget *parent = 0);
    ~CMainWindow();

    CToolbarWidget *getToolBarWidget();
    CTextEditStyleList *getTextEditStyleList();
    CProjectListWidget *getProjectListWidget();
    CFolderTreeView *getFolderTreeView();
    CEditorTabWidget *getEditorTabWidget();
    CDialogFind *getDialogFind();

    void setSaveEnabled(bool bEnabled);
    void setSaveAsEnabled(bool bEnabled);
    void setSaveAllEnabled(bool bEnabled);

    virtual void refreshPosition();
    /**
     * @brief onRename
     * called when file or folder is renamed
     * @param strFrom
     * from filename or folder name)
     * @param strTo
     * to filename or folder name)
     * @param bFile
     * true == file
     * false == folder
     */
    void onRename(QString strFrom, QString strTo, bool bFile);

    void openNewFile(QString strFileName);

    void onOpenDialogFindInFiles(QString strFilePath);
public slots:
    void buttonToolBarNewClicked();
    void openNewFile();

    void onOpenDialogAbout();
    void onOpenDialogFind();
    void onOpenDialogFindInFiles();
    void onOpenDialogReplace();

    void onRunProject();

protected:
    virtual void resizeEvent(QResizeEvent *event);
    virtual void closeEvent(QCloseEvent *e);

private:

    QAction *m_actionSaveFile;
    QAction *m_actionSaveFileAs;
    QAction *m_actionSaveFileAll;
    QAction *m_actionFindNext;
    QAction *m_actionFindPrevious;
    QMenuBar *m_pMenuBar;
    QMenu *m_pMenuFile;
    CToolbarWidget *m_pToolBarWidget;
    CProjectListWidget *m_pProjectListWidget;
    CFolderTreeView *m_pFolderTreeView;
    CEditorTabWidget *m_pEditorTabWidget;
    CTextEditStyleList *m_pTextEditStyleList;
    CSideBar *m_pSideBar;
    CSideBarUpDown *m_pSideBarUpDown;

    CDialogAbout *m_dialogAbout;
    CDialogFind *m_dialogFind;

    void onSearchStart();
};

#endif // CMAINWINDOW_H
