#include <QStackedWidget>
#include <QDebug>
#include <QPushButton>
#include <QDir>
#include <QMessageBox>
#include "cbutton.h"
#include "ceditortabwidget.h"
#include "ceditorwidget.h"
#include "application.h"
#include "cdialogfind.h"
#include "ctoolbarwidget.h"
#include "ceditortabbar.h"
#include "cprojectlistwidget.h"
#include "ctexteditstylelist.h"
#include "cmainwindow.h"
#include "ceditortabbarlabelwidget.h"
#include "ctextedit.h"
#include "cfindfromfilestreeview.h"
#include "csidebarupdown.h"
#include "cfoldertreeview.h"
#include "cprojectwidget.h"

CEditorTabWidget::CEditorTabWidget(CMainWindow *parent) :
    QWidget(parent),
    m_bSideBarPositionDone(false),
    m_pEditorTabBar(NULL),
    m_pStackedEditorWidgets(NULL),
    m_pMainWindow(parent)
{
    m_buttonCloseFindFromFiles = new CButton(this, STRING_PROJECT_CLOSE, "project/close.png");
    m_buttonCloseFindFromFiles->setVisible(false);
    m_pSideBarUpDown = new CSideBarUpDown(this, this);
    m_pSideBarUpDown->setVisible(false);
    m_pSideBarUpDown->setMinPositionY(70);
    m_pEditorTabBar = new CEditorTabBar(this);
    m_pStackedEditorWidgets = new QStackedWidget(this);
    m_pFindFromFilesTreeView = new CFindFromFilesTreeView(m_pMainWindow, this);
    m_pFindFromFilesTreeView->setVisible(false);

    connect(m_pEditorTabBar, SIGNAL(currentChanged(int)), this, SLOT(tabBarCurrentChanged(int)));
    connect(m_pEditorTabBar, SIGNAL(tabCloseRequested(int)), this, SLOT(onTabCloseRequested(int) ) );
    connect(m_buttonCloseFindFromFiles, SIGNAL(clicked()), this, SLOT(onCloseFindFromFilesClicked()));
}

CEditorTabWidget::~CEditorTabWidget()
{
    if (m_buttonCloseFindFromFiles)
    {
        delete m_buttonCloseFindFromFiles;
        m_buttonCloseFindFromFiles = NULL;
    }

    if (m_pEditorTabBar)
    {
        delete m_pEditorTabBar;
        m_pEditorTabBar = NULL;
    }

    if (m_pStackedEditorWidgets)
    {
        delete m_pStackedEditorWidgets;
        m_pStackedEditorWidgets = NULL;
    }
}

void CEditorTabWidget::onFileRename(QString strFrom, QString strTo)
{
    int i, i2;
    CEditorWidget *pEditorWidget = NULL;
    strFrom = QDir::toNativeSeparators(strFrom);
    strTo = QDir::toNativeSeparators(strTo);

    for (i=0;i<m_pStackedEditorWidgets->count();i++)
    {
        pEditorWidget = (CEditorWidget *)m_pStackedEditorWidgets->widget(i);

        if (QDir::toNativeSeparators( pEditorWidget->getFileName() ) == strFrom)
        {
            pEditorWidget->setFileName(strTo);

            for (i2=0;i2<m_pEditorTabBar->count();i2++)
            {
                CEditorTabBarLabelWidget *pWidget = (CEditorTabBarLabelWidget *)m_pEditorTabBar->tabButton(i2, QTabBar::LeftSide);
                if (pEditorWidget->getId() == pWidget->getId())
                {
                    pWidget->setTabName( getTabNameFromFileName(strTo) );
                }
            }
        }
    }

    for (i=0;i<m_listEditorWidget.size();i++)
    {
        pEditorWidget = m_listEditorWidget.at(i);
        if (QDir::toNativeSeparators( pEditorWidget->getFileName() ) == strFrom)
        {
            pEditorWidget->setFileName(strTo);
        }
    }
}


void CEditorTabWidget::tabBarCurrentChanged(int index)
{
    int i;

    CEditorTabBarLabelWidget *pWidget = (CEditorTabBarLabelWidget *)m_pEditorTabBar->tabButton(index, QTabBar::LeftSide);
    CEditorWidget *pEditorWidget = NULL;

    for (i=0;i<m_pStackedEditorWidgets->count();i++)
    {
        pEditorWidget = (CEditorWidget *)m_pStackedEditorWidgets->widget(i);

        if (pEditorWidget->getId() == pWidget->getId())
        {

            m_pStackedEditorWidgets->setCurrentIndex(i);
            m_pMainWindow->setSaveEnabled(!pEditorWidget->isTextSameAsSaved());
            m_pMainWindow->getToolBarWidget()->setComboboxTextStyle( pEditorWidget->getStyleName() );

            CProjectWidget *pProject = m_pMainWindow->getProjectListWidget()->getSelectedItem();
            pProject->setLastOpenIndex(index);

            m_pMainWindow->getFolderTreeView()->setToPath(pEditorWidget->getFileName());
            return;
        }
    }
}
void CEditorTabWidget::setVisibleById(qint64 id, bool bChangeStyle)
{
    int i;
    CEditorWidget *pEditorWidget;

    for (i=0;i<m_pStackedEditorWidgets->count();i++)
    {
        pEditorWidget = (CEditorWidget *)m_pStackedEditorWidgets->widget(i);

        if (pEditorWidget->getId() == id)
        {
            m_pStackedEditorWidgets->setCurrentIndex(i);
            m_pEditorTabBar->setCurrentIndex(i);

            m_pMainWindow->setSaveEnabled(!pEditorWidget->isTextSameAsSaved());
            if (bChangeStyle)
            {
                m_pMainWindow->getToolBarWidget()->setComboboxTextStyle( pEditorWidget->getStyleName() );
            }
            return;
        }
    }
}

void CEditorTabWidget::textChanged(qint64 iId, bool bTextIsSame)
{
    int i;
    CEditorWidget *pEditorWidget;

    for (i=0;i<m_pStackedEditorWidgets->count();i++)
    {
        pEditorWidget = (CEditorWidget *)m_pStackedEditorWidgets->widget(i);

        if (pEditorWidget->getId() == iId)
        {
            CEditorTabBarLabelWidget *pTabWidget = (CEditorTabBarLabelWidget *)m_pEditorTabBar->tabButton(i, QTabBar::LeftSide);
            pTabWidget->setTextIsChanged(bTextIsSame);
            m_pMainWindow->setSaveEnabled(!bTextIsSame);
            m_pMainWindow->getToolBarWidget()->setComboboxTextStyle( pEditorWidget->getStyleName() );
            return;
        }
    }


}


void CEditorTabWidget::onOpenFile(QString strFileName)
{
    int i;

    for (i=0;i<m_listEditorWidget.size();i++)
    {
        if (m_listEditorWidget.at(i)->getFileName() == strFileName)
        {
            setProjectListForFileName(strFileName);
            setVisibleById(m_listEditorWidget.at(i)->getId());
            m_pMainWindow->setSaveAsEnabled(true);
            m_pMainWindow->getToolBarWidget()->setComboboxTextStyle( m_listEditorWidget.at(i)->getStyleName() );

            return;
        }
    }

    CEditorWidget *pEditorWidget = new CEditorWidget(m_pMainWindow, this, strFileName );
    pEditorWidget->setId( CEditorTabBarLabelWidget::getNewId() );
    pEditorWidget->setVisible(false);
    m_listEditorWidget.append( pEditorWidget );

    setProjectListForFileName(strFileName);
    setVisibleById(pEditorWidget->getId(), false);
    m_pMainWindow->setSaveAsEnabled(true);
}

void CEditorTabWidget::setProjectListForFileName(QString strFileName)
{
    m_pMainWindow->getProjectListWidget()->setProjectListForFileName(strFileName);
}

QString CEditorTabWidget::getTabNameFromFileName(const QString &strFileName)
{
    QString strTabName;

    if (strFileName.isEmpty())
    {
        return STRING_UNTITLED;
    }

    int iLastIndex = strFileName.lastIndexOf("/");
    if (iLastIndex < strFileName.lastIndexOf("\\"))
    {
        iLastIndex = strFileName.lastIndexOf("\\");
    }

    strTabName = strFileName.right( strFileName.size() - iLastIndex - 1 );

    return strTabName;
}


void CEditorTabWidget::addNewTab()
{
    CEditorWidget *pEditorWidget = new CEditorWidget(m_pMainWindow, this, "" );
    pEditorWidget->setId( CEditorTabBarLabelWidget::getNewId() );
    pEditorWidget->setVisible(false);
    m_listEditorWidget.append( pEditorWidget );

    setProjectListForFileName("");
    setVisibleById(pEditorWidget->getId());

    m_pMainWindow->setSaveAsEnabled(true);

    m_pMainWindow->getToolBarWidget()->setComboboxTextStyle( pEditorWidget->getStyleName() );

}


void CEditorTabWidget::setWidgetsForFolder(QString strFolderPath, int iTabOpenIndex)
{
    while (1)
    {
        if (m_pStackedEditorWidgets->count() == 0)
        {
            break;
        }
        removeWidget(0, false);
    }


    int i;
    int iFirstIndex = -1;
    CEditorWidget *pEditorWidget;

    for (i=0;i<m_listEditorWidget.size();i++)
    {
        pEditorWidget = m_listEditorWidget.at(i);
        if (CProjectListWidget::isFileNameValidForFolderProject(pEditorWidget->getFileName(), strFolderPath))
        {
            int iIndex = m_pEditorTabBar->addTab( getTabNameFromFileName(pEditorWidget->getFileName()) );
            CEditorTabBarLabelWidget *pWidget = (CEditorTabBarLabelWidget *)m_pEditorTabBar->tabButton(iIndex, QTabBar::LeftSide);
            pWidget->setId( pEditorWidget->getId() );
            m_pStackedEditorWidgets->addWidget( pEditorWidget );
            textChanged(pEditorWidget->getId(), pEditorWidget->isTextSameAsSaved() );

            if (iFirstIndex == -1)
            {
                iFirstIndex = i;
            }
        }
    }

    if (m_pStackedEditorWidgets->count() > 0)
    {
        if (iTabOpenIndex < 0 || iTabOpenIndex >= m_pStackedEditorWidgets->count()) {
            iTabOpenIndex = 0;
        }

        m_pStackedEditorWidgets->setCurrentIndex(iTabOpenIndex);
        m_pEditorTabBar->setCurrentIndex(iTabOpenIndex);
        pEditorWidget = m_listEditorWidget.at(iFirstIndex);
        m_pMainWindow->setSaveEnabled(!pEditorWidget->isTextSameAsSaved());
        m_pMainWindow->setSaveAsEnabled(true);

        m_pMainWindow->getToolBarWidget()->setComboboxTextStyle( pEditorWidget->getStyleName() );
    }
    else
    {
        m_pMainWindow->setSaveEnabled(false);
        m_pMainWindow->setSaveAsEnabled(false);
    }
}

void CEditorTabWidget::setTextEditStyle(CTextEditStyle *style)
{
    CEditorWidget *pEditorWidget = NULL;

    int iIndex = m_pStackedEditorWidgets->currentIndex();

    if (iIndex >= 0 && iIndex < m_pStackedEditorWidgets->count())
    {
        pEditorWidget = (CEditorWidget *)m_pStackedEditorWidgets->widget(iIndex);
        pEditorWidget->setTextEditStyle(style);
    }
}

void CEditorTabWidget::refreshPosition()
{
    m_pEditorTabBar->setGeometry(0,0, width(), 30);
    if (m_pFindFromFilesTreeView->isVisible())
    {
        m_pSideBarUpDown->setGeometry(0, m_pSideBarUpDown->getPositionY(), width(), 5);
        m_buttonCloseFindFromFiles->setGeometry(width()-20,m_pSideBarUpDown->y()+m_pSideBarUpDown->height(),
                                                20,20);
        m_pFindFromFilesTreeView->setGeometry(0, m_buttonCloseFindFromFiles->y() + m_buttonCloseFindFromFiles->height() + 5,
                                              width(), height() - (m_buttonCloseFindFromFiles->y() + m_buttonCloseFindFromFiles->height() + 5)) ;
        m_pStackedEditorWidgets->setGeometry(0,
                                             m_pEditorTabBar->y() + m_pEditorTabBar->height(),
                                             width(),
                                             m_pSideBarUpDown->y()  - (m_pEditorTabBar->y() + m_pEditorTabBar->height())  );
    }
    else
    {
        m_pFindFromFilesTreeView->setGeometry(0, height(), width(), 0);
        m_pStackedEditorWidgets->setGeometry(0,
                                             m_pEditorTabBar->y() + m_pEditorTabBar->height(),
                                             width(),
                                             height() - m_pEditorTabBar->y() - m_pEditorTabBar->height() );
    }
}

void CEditorTabWidget::resizeEvent(QResizeEvent *e)
{
    if (e)
    {
        QWidget::resizeEvent(e);
    }

    refreshPosition();

}

void CEditorTabWidget::redo() {


    CEditorWidget *pEditorWidget = NULL;

    int iIndex = m_pStackedEditorWidgets->currentIndex();

    if (iIndex >= 0 && iIndex < m_pStackedEditorWidgets->count())
    {
        pEditorWidget = (CEditorWidget *)m_pStackedEditorWidgets->widget(iIndex);
        pEditorWidget->redo();
    }
}

void CEditorTabWidget::save()
{
    CEditorWidget *pEditorWidget = NULL;

    int iIndex = m_pStackedEditorWidgets->currentIndex();

    if (iIndex >= 0 && iIndex < m_pStackedEditorWidgets->count())
    {
        pEditorWidget = (CEditorWidget *)m_pStackedEditorWidgets->widget(iIndex);
        if (pEditorWidget->save())
        {
            setEditorWidgetTabColor(pEditorWidget->getId());
        }
    }
}

void CEditorTabWidget::saveAs()
{
    CEditorWidget *pEditorWidget = NULL;

    int iIndex = m_pStackedEditorWidgets->currentIndex();

    if (iIndex >= 0 && iIndex < m_pStackedEditorWidgets->count())
    {
        pEditorWidget = (CEditorWidget *)m_pStackedEditorWidgets->widget(iIndex);
        if (pEditorWidget->save(true))
        {
            setEditorWidgetTabColor(pEditorWidget->getId());
        }
    }
}

void CEditorTabWidget::setEditorWidgetTabColor(qint64 iId)
{
    int i;
    CEditorWidget *pEditorWidget = NULL;

    for (i=0;i<m_pStackedEditorWidgets->count();i++)
    {
        pEditorWidget = (CEditorWidget *)m_pStackedEditorWidgets->widget(i);

        if (pEditorWidget->getId() == iId)
        {
            CEditorTabBarLabelWidget *pTabWidget = (CEditorTabBarLabelWidget *)m_pEditorTabBar->tabButton(i, QTabBar::LeftSide);
            pTabWidget->setTextIsChanged(pEditorWidget->isTextSameAsSaved());

            if (m_pEditorTabBar->currentIndex() == i)
            {
                m_pMainWindow->setSaveEnabled(!pEditorWidget->isTextSameAsSaved());
                pTabWidget->setTabName( getTabNameFromFileName(pEditorWidget->getFileName()) );
            }
            return;
        }
    }

}

void CEditorTabWidget::saveAll()
{
    int i;
    bool bOneSaveFailed = false;
    CEditorWidget *pEditorWidget = NULL;

    for (i=0;i<m_listEditorWidget.size() && bOneSaveFailed == false;i++)
    {
        pEditorWidget = (CEditorWidget *)m_listEditorWidget.at(i);

        if (pEditorWidget->isTextSameAsSaved() == false &&
            pEditorWidget->getFileName().isEmpty() == false)
        {
            if (pEditorWidget->save())
            {
                setEditorWidgetTabColor(pEditorWidget->getId());
            }
            else
            {
                bOneSaveFailed = true;
            }
        }
    }

    for (i=0;i<m_listEditorWidget.size() && bOneSaveFailed == false;i++)
    {
        pEditorWidget = (CEditorWidget *)m_listEditorWidget.at(i);

        if (pEditorWidget->isTextSameAsSaved() == false &&
            pEditorWidget->getFileName().isEmpty() == true)
        {
            if (pEditorWidget->save())
            {
                setEditorWidgetTabColor(pEditorWidget->getId());
            }
            else
            {
                bOneSaveFailed = true;
            }
        }
    }


    if (bOneSaveFailed == false)
    {
        m_pMainWindow->setSaveAllEnabled(false);
    }
}

void CEditorTabWidget::searchNext(bool bNewSearch)
{
    CDialogFind *pDialogFind = m_pMainWindow->getDialogFind();
    CEditorWidget *pEditorWidget, *pEditorWidgetNextSearch;
    CTextEdit::ESearchResult result;
    Qt::CaseSensitivity caseSensitivity = pDialogFind->getCaseSensitivity();
    int i;

    if (m_pStackedEditorWidgets->count() == 0)
    {
        return;
    }

    switch (pDialogFind->getType())
    {
        case CDialogFind::DIALOG_FIND_TYPE_FIND:
            pEditorWidget = (CEditorWidget *)m_pStackedEditorWidgets->currentWidget();
            result = pEditorWidget->getTextEdit()->searchNext(pDialogFind->getFindText(), caseSensitivity, bNewSearch);

            if (pDialogFind->getSearchType() == CDialogFind::EDIALOG_FIND_SEARCH_TYPE_ALL_OPEN_DOCUMENTS &&
                (result == CTextEdit::SEARCH_RESULT_ALL_FOUND_ALREADY ||
                result == CTextEdit::SEARCH_RESULT_NOT_FOUND))
            {
                for (i=m_pStackedEditorWidgets->currentIndex()+1;i<m_pStackedEditorWidgets->count();i++)
                {
                    pEditorWidgetNextSearch = (CEditorWidget *)m_pStackedEditorWidgets->widget(i);
                    if (pEditorWidgetNextSearch->getTextEdit()->toPlainText().indexOf(pDialogFind->getFindText(), 0, caseSensitivity) != -1)
                    {
                        setVisibleById(pEditorWidgetNextSearch->getId(), true);
                        pEditorWidgetNextSearch->getTextEdit()->searchNext(pDialogFind->getFindText(), caseSensitivity, true);
                        return;
                    }
                }
                for (i=0;i<m_pStackedEditorWidgets->currentIndex();i++)
                {
                    pEditorWidgetNextSearch = (CEditorWidget *)m_pStackedEditorWidgets->widget(i);
                    if (pEditorWidgetNextSearch->getTextEdit()->toPlainText().indexOf(pDialogFind->getFindText(), 0, caseSensitivity) != -1)
                    {
                        setVisibleById(pEditorWidgetNextSearch->getId(), true);
                        pEditorWidgetNextSearch->getTextEdit()->searchNext(pDialogFind->getFindText(), caseSensitivity, true);
                        return;
                    }
                }
            }

            if (result == CTextEdit::SEARCH_RESULT_ALL_FOUND_ALREADY)
            {
                pEditorWidget->getTextEdit()->searchNext(pDialogFind->getFindText(), caseSensitivity, true, 1);
            }

            break;
        default:
            break;
    }
}

QString CEditorTabWidget::getSelectedText() const
{
    if (m_pStackedEditorWidgets->count() > 0)
    {
        CEditorWidget *pEditorWidget = (CEditorWidget *)m_pStackedEditorWidgets->currentWidget();
        return pEditorWidget->getSelectedText();
    }

    return "";
}



void CEditorTabWidget::searchPrevious()
{
    int i;
    CDialogFind *pDialogFind = m_pMainWindow->getDialogFind();
    CEditorWidget *pEditorWidget, *pEditorWidgetNextSearch;
    CTextEdit::ESearchResult result;
    Qt::CaseSensitivity caseSensitivity = pDialogFind->getCaseSensitivity();

    if (m_pStackedEditorWidgets->count() == 0)
    {
        return;
    }


    switch (pDialogFind->getType())
    {
        case CDialogFind::DIALOG_FIND_TYPE_FIND:
            pEditorWidget = (CEditorWidget *)m_pStackedEditorWidgets->currentWidget();
            result = pEditorWidget->getTextEdit()->searchPrevious(pDialogFind->getFindText(), caseSensitivity, false);

            if (pDialogFind->getSearchType() == CDialogFind::EDIALOG_FIND_SEARCH_TYPE_ALL_OPEN_DOCUMENTS &&
                (result == CTextEdit::SEARCH_RESULT_ALL_FOUND_ALREADY ||
                result == CTextEdit::SEARCH_RESULT_NOT_FOUND))
            {
                for (i=m_pStackedEditorWidgets->currentIndex()-1;i>=0;i--)
                {
                    pEditorWidgetNextSearch = (CEditorWidget *)m_pStackedEditorWidgets->widget(i);
                    if (pEditorWidgetNextSearch->getTextEdit()->toPlainText().indexOf(pDialogFind->getFindText(), 0, caseSensitivity) != -1)
                    {
                        setVisibleById(pEditorWidgetNextSearch->getId(), true);
                        pEditorWidgetNextSearch->getTextEdit()->searchPrevious(pDialogFind->getFindText(), caseSensitivity, true);
                        return;
                    }
                }
                for (i=m_pStackedEditorWidgets->count()-1;i>m_pStackedEditorWidgets->currentIndex();i--)
                {
                    pEditorWidgetNextSearch = (CEditorWidget *)m_pStackedEditorWidgets->widget(i);
                    if (pEditorWidgetNextSearch->getTextEdit()->toPlainText().indexOf(pDialogFind->getFindText(), 0, caseSensitivity) != -1)
                    {
                        setVisibleById(pEditorWidgetNextSearch->getId(), true);
                        pEditorWidgetNextSearch->getTextEdit()->searchPrevious(pDialogFind->getFindText(), caseSensitivity, true);
                        return;
                    }
                }
            }

            if (result == CTextEdit::SEARCH_RESULT_ALL_FOUND_ALREADY)
            {
                pEditorWidget->getTextEdit()->searchPrevious(pDialogFind->getFindText(), caseSensitivity, true);
            }

            break;
        default:
            break;
    }
}

void CEditorTabWidget::searchFromFiles()
{
    m_pFindFromFilesTreeView->searchFromFiles();

    if (!m_pFindFromFilesTreeView->isVisible())
    {
        m_pSideBarUpDown->setVisible(true);
        m_pFindFromFilesTreeView->setVisible(true);
        m_buttonCloseFindFromFiles->setVisible(true);

        if (m_bSideBarPositionDone == false)
        {
            m_pSideBarUpDown->setPositionY( this->height() - 200 );
            m_bSideBarPositionDone = true;
        }
        resizeEvent(NULL);
    }
}

void CEditorTabWidget::onCloseFindFromFilesClicked()
{
    m_pSideBarUpDown->setVisible(false);
    m_pFindFromFilesTreeView->setVisible(false);
    m_buttonCloseFindFromFiles->setVisible(false);

    resizeEvent(NULL);
}

void CEditorTabWidget::replace()
{
    int i;
    CEditorWidget *pEditorWidget;
    CDialogFind *pDialogFind = m_pMainWindow->getDialogFind();

    if (m_pStackedEditorWidgets->count() == 0)
    {
        return;
    }

    if (pDialogFind->getSearchType() == CDialogFind::EDIALOG_FIND_SEARCH_TYPE_ALL_OPEN_DOCUMENTS)
    {
        for (i=0;i<m_pStackedEditorWidgets->count();i++)
        {
            pEditorWidget = (CEditorWidget *)m_pStackedEditorWidgets->widget(i);
            pEditorWidget->getTextEdit()->replace();
        }
    }
    else
    {
        pEditorWidget = (CEditorWidget *)m_pStackedEditorWidgets->currentWidget();
        pEditorWidget->getTextEdit()->replace();
    }
}

void CEditorTabWidget::setCursorPosition(int iLineNumber, QString strToText)
{
    CEditorWidget *pEditorWidget;

    if (m_pStackedEditorWidgets->count() == 0)
    {
        return;
    }

    pEditorWidget = (CEditorWidget *)m_pStackedEditorWidgets->currentWidget();
    pEditorWidget->getTextEdit()->setCursorPosition(iLineNumber, strToText);
}

void CEditorTabWidget::onTabCloseRequested(int index)
{
    removeWidget(index, true);
}

void CEditorTabWidget::removeWidget(int iIndex, bool bRemoveFromEditorList)
{
    int iRemoveFromEditorList = -1;
    int i;
    CEditorWidget *pEditorWidget = NULL;
    CEditorWidget *pCmpEditorWidget = NULL;

    if (bRemoveFromEditorList)
    {
        pEditorWidget = (CEditorWidget *)m_pStackedEditorWidgets->widget(iIndex);

        if (!pEditorWidget->isTextSameAsSaved())
        {
            QMessageBox messageBox;
            messageBox.setText(STRING_DOCUMENT_HAS_BEEN_MODIFIED);
            messageBox.setInformativeText(STRING_DO_YOU_WANT_SAVE_YOUR_CHANGES);
            messageBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
            messageBox.setDefaultButton(QMessageBox::Save);
            switch (messageBox.exec())
            {
                case QMessageBox::Save:
                    pEditorWidget->save();
                    break;
                case QMessageBox::Discard:
                    break;
                case QMessageBox::Cancel:
                default:
                    return;
            }

        }

        for (i=0;i<m_listEditorWidget.size();i++)
        {
            pCmpEditorWidget = m_listEditorWidget.at(i);

            if (pCmpEditorWidget->getId() == pEditorWidget->getId())
            {
                iRemoveFromEditorList = i;
                break;
            }
        }
    }

    m_pStackedEditorWidgets->removeWidget( m_pStackedEditorWidgets->widget(iIndex) );
    m_pEditorTabBar->removeTab(iIndex);

    if (iRemoveFromEditorList != -1)
    {
        pEditorWidget = m_listEditorWidget.at(iRemoveFromEditorList);
        m_pMainWindow->getProjectListWidget()->closeFile(pEditorWidget->getFileName());
        delete pEditorWidget;
        m_listEditorWidget.removeAt(iRemoveFromEditorList);
    }
}

bool CEditorTabWidget::onClose()
{
    int i;
    CEditorWidget *pEditorWidget = NULL;
    bool bQueryDone = false;
    bool bSave = false;

    for (i=0;i<m_listEditorWidget.count();i++)
    {
        pEditorWidget = m_listEditorWidget.at(i);

        if (!pEditorWidget->isTextSameAsSaved())
        {
            if (bQueryDone == false)
            {
                QMessageBox messageBox;
                messageBox.setText(STRING_ONE_OF_DOCUMENT_HAS_BEEN_MODIFIED);
                messageBox.setInformativeText(STRING_DO_YOU_WANT_SAVE_YOUR_CHANGES);

                messageBox.addButton(tr("Choose one by one"), QMessageBox::ActionRole);

                messageBox.setStandardButtons(QMessageBox::YesToAll | QMessageBox::NoToAll | QMessageBox::Cancel);
                messageBox.setDefaultButton(QMessageBox::YesToAll);

                switch (messageBox.exec())
                {
                    case QMessageBox::YesToAll:
                        bSave = true;
                        break;
                    case QMessageBox::NoToAll:
                        break;
                    case QMessageBox::Cancel:
                    default:
                        return false;
                }

                bQueryDone = true;
            }

            if (bSave)
            {
                pEditorWidget->save();
            }
        }
    }

    for (i=0;i<m_listEditorWidget.count();i++)
    {
        pEditorWidget = m_listEditorWidget.at(i);
        m_pMainWindow->getProjectListWidget()->closeFile(pEditorWidget->getFileName());
    }

    return true;
}
