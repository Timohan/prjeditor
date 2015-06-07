#include <QResizeEvent>
#include "cdialogfind.h"
#include "cmainwindow.h"
#include "ui_cmainwindow.h"
#include "ctoolbarwidget.h"
#include "cprojectlistwidget.h"
#include "cfoldertreeview.h"
#include "ceditortabwidget.h"
#include "application.h"
#include "ctexteditstylelist.h"
#include "ctexteditstyle.h"
#include "ctextedithighlightrule.h"
#include "cmainwindow.h"
#include "csidebar.h"
#include "csidebarupdown.h"
#include "cdialogabout.h"
#include <QDebug>
#include <QColor>
#include <QMenuBar>
#include <QFileDialog>
#include <QAction>

CMainWindow::CMainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_pToolBarWidget(NULL),
    m_pProjectListWidget(NULL),
    m_pFolderTreeView(NULL),
    m_pEditorTabWidget(NULL),
    m_pTextEditStyleList(NULL)
{
    setWindowTitle(STRING_APPLICATION_NAME);
    m_pMenuBar = this->menuBar();
    m_pMenuBar->setNativeMenuBar(false);
    m_pMenuFile = m_pMenuBar->addMenu(STRING_MENU_FILE);
    QAction *actionNewFile = m_pMenuFile->addAction(STRING_MENU_FILE_NEW_FILE);
    QAction *actionOpenFile = m_pMenuFile->addAction(STRING_MENU_FILE_OPEN_FILE);
    m_actionSaveFile = m_pMenuFile->addAction(STRING_MENU_FILE_SAVE_FILE);
    m_actionSaveFileAs = m_pMenuFile->addAction(STRING_MENU_FILE_SAVE_AS);
    m_actionSaveFileAll = m_pMenuFile->addAction(STRING_MENU_FILE_SAVE_ALL);

    m_dialogFind = new CDialogFind(this);
    m_dialogAbout = new CDialogAbout(this);

    m_actionSaveFile->setEnabled(false);
    m_actionSaveFileAs->setEnabled(false);
    m_actionSaveFileAll->setEnabled(false);
    QMenu *editMenu = m_pMenuBar->addMenu(STRING_MENU_EDIT);

    QAction *actionUndo = editMenu->addAction(STRING_MENU_UNDO);
    QAction *actionRedo = editMenu->addAction(STRING_MENU_REDO);
    editMenu->addSeparator();
    QAction *actionCut = editMenu->addAction(STRING_MENU_CUT);
    QAction *actionCopy = editMenu->addAction(STRING_MENU_COPY);
    QAction *actionPaste = editMenu->addAction(STRING_MENU_PASTE);

    QMenu *searchMenu = m_pMenuBar->addMenu(STRING_MENU_SEARCH);
    QAction *actionFind = searchMenu->addAction(STRING_MENU_FIND);
    m_actionFindNext = searchMenu->addAction(STRING_MENU_FIND_NEXT);
    m_actionFindPrevious = searchMenu->addAction(STRING_MENU_FIND_PREVIOUS);
    m_actionFindNext->setEnabled(false);
    m_actionFindPrevious->setEnabled(false);

    searchMenu->addSeparator();
    QAction *actionFindInFiles = searchMenu->addAction(STRING_MENU_FIND_IN_FILES);
    searchMenu->addSeparator();
    QAction *actionReplace = searchMenu->addAction(STRING_MENU_REPLACE);

    QMenu *projectMenu = m_pMenuBar->addMenu(STRING_PROJECT);
    QAction *runProject = projectMenu->addAction(STRING_RUN_PROJECT);


    QMenu *helpMenu = m_pMenuBar->addMenu(STRING_MENU_HELP);
    QAction *actionAbout = helpMenu->addAction(STRING_MENU_ABOUT.arg(STRING_APPLICATION_NAME));

    m_pSideBar = new CSideBar(this, NULL);
    m_pEditorTabWidget = new CEditorTabWidget(this);
    m_pFolderTreeView = new CFolderTreeView(this);
    m_pToolBarWidget = new CToolbarWidget(this);
    m_pTextEditStyleList = new CTextEditStyleList(this);
    m_pProjectListWidget = new CProjectListWidget(this);
    m_pSideBarUpDown = new CSideBarUpDown(this, this);
    m_pSideBarUpDown->setPositionY(200);
    m_pFolderTreeView->setProjectListWidget(m_pProjectListWidget);
    this->resize(800,600);

    connect(actionNewFile, SIGNAL(triggered()), this, SLOT(buttonToolBarNewClicked()));
    connect(actionOpenFile, SIGNAL(triggered()), this, SLOT(openNewFile()));
    connect(m_pToolBarWidget, SIGNAL(openNewFile()), this, SLOT(openNewFile()));
    connect(m_pToolBarWidget, SIGNAL(onButtonNewClicked()), SLOT(buttonToolBarNewClicked()));
    connect(m_pFolderTreeView, SIGNAL(openFile(QString)), m_pEditorTabWidget, SLOT(onOpenFile(QString)));

    connect(m_actionSaveFile, SIGNAL(triggered()), m_pEditorTabWidget, SLOT(save()));
    connect(m_actionSaveFileAll, SIGNAL(triggered()), m_pEditorTabWidget, SLOT(saveAll()));
    connect(m_pToolBarWidget, SIGNAL(onSave()), m_pEditorTabWidget, SLOT(save()));
    connect(m_pToolBarWidget, SIGNAL(onSaveAll()), m_pEditorTabWidget, SLOT(saveAll()));
    connect(m_actionSaveFileAs, SIGNAL(triggered()), m_pEditorTabWidget, SLOT(saveAs()));
    connect(m_actionFindNext, SIGNAL(triggered()), m_pEditorTabWidget, SLOT(searchNext()));
    connect(m_actionFindPrevious, SIGNAL(triggered()), m_pEditorTabWidget, SLOT(searchPrevious()));

    connect(actionAbout, SIGNAL(triggered()), this, SLOT(onOpenDialogAbout()));
    connect(actionFind, SIGNAL(triggered()), this, SLOT(onOpenDialogFind()));
    connect(actionFindInFiles, SIGNAL(triggered()), this, SLOT(onOpenDialogFindInFiles()));
    connect(actionReplace, SIGNAL(triggered()), this, SLOT(onOpenDialogReplace()));
    connect(runProject, SIGNAL(triggered()), this, SLOT(onRunProject()));

    actionNewFile->setShortcut( QKeySequence(SHORTCUT_KEY_NEW) );
    actionOpenFile->setShortcut( QKeySequence(SHORTCUT_KEY_OPEN) );

    m_actionSaveFile->setShortcut( QKeySequence(SHORTCUT_KEY_SAVE) );
    m_actionSaveFileAll->setShortcut( QKeySequence(SHORTCUT_KEY_SAVE_ALL) );

    actionUndo->setShortcut( QKeySequence(SHORTCUT_KEY_UNDO) );
    actionRedo->setShortcut( QKeySequence(SHORTCUT_KEY_REDO) );
    actionCut->setShortcut( QKeySequence(SHORTCUT_KEY_CUT) );
    actionCopy->setShortcut( QKeySequence(SHORTCUT_KEY_COPY) );
    actionPaste->setShortcut( QKeySequence(SHORTCUT_KEY_PASTE) );

    actionFind->setShortcut( QKeySequence(SHORTCUT_KEY_FIND) );
    actionFindInFiles->setShortcut( QKeySequence(SHORTCUT_KEY_FIND_IN_FILES) );
    actionReplace->setShortcut( QKeySequence(SHORTCUT_KEY_REPLACE) );
    m_actionFindNext->setShortcut( QKeySequence( SHORTCUT_KEY_FIND_NEXT ) );
    m_actionFindPrevious->setShortcut( QKeySequence( SHORTCUT_KEY_FIND_PREVIOUS ) );

    runProject->setShortcut( QKeySequence(SHORTCUT_KEY_RUN_PROJECT) );
}

CMainWindow::~CMainWindow()
{
    if (m_pToolBarWidget)
    {
        delete m_pToolBarWidget;
        m_pToolBarWidget = NULL;
    }

    if (m_pProjectListWidget)
    {
        delete m_pProjectListWidget;
        m_pProjectListWidget = NULL;
    }

    if (m_pFolderTreeView)
    {
        delete m_pFolderTreeView;
        m_pFolderTreeView = NULL;
    }

    if (m_pEditorTabWidget)
    {
        delete m_pEditorTabWidget;
        m_pEditorTabWidget = NULL;
    }

    if (m_pTextEditStyleList)
    {
        delete m_pTextEditStyleList;
        m_pTextEditStyleList = NULL;
    }

    if (m_pMenuFile)
    {
        delete m_pMenuFile;
        m_pMenuFile = NULL;
    }

    if (m_dialogAbout) {
        delete m_dialogAbout;
        m_dialogAbout = NULL;
    }

    if (m_dialogFind) {
        delete m_dialogFind;
        m_dialogFind = NULL;
    }
}

CToolbarWidget *CMainWindow::getToolBarWidget()
{
    return m_pToolBarWidget;
}

CTextEditStyleList *CMainWindow::getTextEditStyleList()
{
    return m_pTextEditStyleList;
}

CProjectListWidget *CMainWindow::getProjectListWidget()
{
    return m_pProjectListWidget;
}

CFolderTreeView *CMainWindow::getFolderTreeView()
{
    return m_pFolderTreeView;
}
CEditorTabWidget *CMainWindow::getEditorTabWidget()
{
    return m_pEditorTabWidget;
}

CDialogFind *CMainWindow::getDialogFind()
{
    return m_dialogFind;
}

void CMainWindow::buttonToolBarNewClicked()
{
    m_pEditorTabWidget->addNewTab();
}

void CMainWindow::refreshPosition()
{
    if (m_pToolBarWidget)
    {
        m_pMenuBar->setGeometry(0,0, width(), 21);

        m_pToolBarWidget->setGeometry(0,m_pMenuBar->y()+m_pMenuBar->height(), width(), m_pToolBarWidget->getDefaultHeight());
        m_pProjectListWidget->setGeometry(5,
                m_pToolBarWidget->y()+m_pToolBarWidget->height()+5,
                m_pSideBar->getPositionX()-6,
                m_pSideBarUpDown->getPositionY() - 1 - (m_pToolBarWidget->y()+m_pToolBarWidget->height()+5));

        m_pSideBarUpDown->setGeometry(0, m_pSideBarUpDown->getPositionY(), m_pSideBar->getPositionX()-6, 5);

        int y = m_pSideBarUpDown->y()+m_pSideBarUpDown->height()+1;
        m_pFolderTreeView->setGeometry(5, y,
                            m_pSideBar->getPositionX()-6, this->height()-y-5);

        m_pSideBar->setGeometry(m_pSideBar->getPositionX(),
                                m_pProjectListWidget->y(),
                                SIDE_BAR_WIDTH,
                                this->height() - 5 - m_pProjectListWidget->y());

        m_pEditorTabWidget->setGeometry(m_pSideBar->x() + m_pSideBar->width() + 1,
                                     m_pProjectListWidget->y(),
                                     this->width() - (m_pFolderTreeView->x() + m_pFolderTreeView->width()) - 10,
                                     this->height() - 5 - m_pProjectListWidget->y()
                                     );
    }
}

void CMainWindow::resizeEvent(QResizeEvent *event)
{   
    if (event)
    {
        QMainWindow::resizeEvent(event);
    }

    refreshPosition();
}

void CMainWindow::openNewFile()
{
    QString strFileName = QFileDialog::getOpenFileName(this, tr("Open File"));
    if (strFileName.isEmpty() == false)
    {
        openNewFile(strFileName);
    }
}
void CMainWindow::openNewFile(QString strFileName)
{
    if (strFileName.isEmpty() == false)
    {
        m_pFolderTreeView->openFile(strFileName);
        m_pProjectListWidget->openFile(strFileName);
    }
}
void CMainWindow::setSaveEnabled(bool bEnabled)
{
    m_actionSaveFile->setEnabled(bEnabled);
    m_pToolBarWidget->setSaveEnabled(bEnabled);

    if (bEnabled)
    {
        m_actionSaveFileAs->setEnabled(true);
        m_actionSaveFileAll->setEnabled(true);
    }
}

void CMainWindow::setSaveAsEnabled(bool bEnabled)
{
    m_actionSaveFileAs->setEnabled(bEnabled);
}

void CMainWindow::setSaveAllEnabled(bool bEnabled)
{
    m_actionSaveFileAll->setEnabled(bEnabled);
    m_pToolBarWidget->setSaveAllEnabled(bEnabled);
}

void CMainWindow::onSearchStart()
{
    switch (m_dialogFind->getType())
    {
        case CDialogFind::DIALOG_FIND_TYPE_FIND:
            m_pEditorTabWidget->searchNext(true);
            m_actionFindNext->setEnabled(true);
            m_actionFindPrevious->setEnabled(true);
            break;

        case CDialogFind::DIALOG_FIND_TYPE_FIND_IN_FILES:
            m_pEditorTabWidget->searchFromFiles();
            m_actionFindNext->setEnabled(false);
            m_actionFindPrevious->setEnabled(false);
            break;
        case CDialogFind::DIALOG_FIND_TYPE_FIND_REPLACE:
            m_pEditorTabWidget->replace();
            m_actionFindNext->setEnabled(false);
            m_actionFindPrevious->setEnabled(false);
            break;
    }

    m_dialogFind->setSearchValuesAsTemplate();
}

void CMainWindow::onOpenDialogAbout() {

    m_dialogAbout->exec();
}

void CMainWindow::onOpenDialogFind()
{
    m_actionFindNext->setEnabled(false);
    m_actionFindPrevious->setEnabled(false);
    QString strSelectedText = m_pEditorTabWidget->getSelectedText();
    m_dialogFind->setSelectedTextToFind(strSelectedText);
    m_dialogFind->setType(CDialogFind::DIALOG_FIND_TYPE_FIND);
    if (m_dialogFind->exec() == 1)
    {
        onSearchStart();
    }
}

void CMainWindow::onOpenDialogFindInFiles(QString strFilePath)
{
    m_actionFindNext->setEnabled(false);
    m_actionFindPrevious->setEnabled(false);
    QString strSelectedText = m_pEditorTabWidget->getSelectedText();
    m_dialogFind->setSelectedTextToFind(strSelectedText);
    m_dialogFind->setType(CDialogFind::DIALOG_FIND_TYPE_FIND_IN_FILES);

    if (strFilePath.isEmpty() == false) {
        m_dialogFind->setFindDirName(strFilePath);
    }

    if (m_dialogFind->exec() == 1)
    {
        onSearchStart();
    }
}

void CMainWindow::onOpenDialogFindInFiles()
{
    onOpenDialogFindInFiles("");
}

void CMainWindow::onOpenDialogReplace()
{
    m_actionFindNext->setEnabled(false);
    m_actionFindPrevious->setEnabled(false);
    QString strSelectedText = m_pEditorTabWidget->getSelectedText();
    m_dialogFind->setSelectedTextToFind(strSelectedText);
    m_dialogFind->setType(CDialogFind::DIALOG_FIND_TYPE_FIND_REPLACE);
    if (m_dialogFind->exec() == 1)
    {
        onSearchStart();
    }
}
void CMainWindow::closeEvent(QCloseEvent *e)
{
    if (m_pEditorTabWidget->onClose()) {
        e->accept();
    } else {
        e->ignore();
    }
}

void CMainWindow::onRename(QString strFrom, QString strTo, bool bFile)
{
    if (bFile == false)
    {
        m_pProjectListWidget->onFolderRename(strFrom, strTo);
    }
    else
    {
        m_pEditorTabWidget->onFileRename(strFrom, strTo);
    }
}

void CMainWindow::onRunProject()
{
    m_pProjectListWidget->onRunProject();
}
