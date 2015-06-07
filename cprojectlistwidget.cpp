#include <QDebug>
#include <QSettings>
#include <QDir>
#include "application.h"
#include "ctoolbarwidget.h"
#include "cprojectlistwidget.h"
#include "cprojectwidget.h"
#include "cfoldertreeview.h"
#include "cmainwindow.h"
#include "ceditortabwidget.h"

CProjectListWidget::CProjectListWidget(CMainWindow *parent) :
    QListWidget(parent),
    m_pMainWindow(parent)
{
    connect(this, SIGNAL(itemSelectionChanged()), this, SLOT(selectedProjectChanged()));

    loadSettings();
}

CProjectListWidget::~CProjectListWidget()
{
    saveSettings();
}

void CProjectListWidget::loadSettings()
{
    QListWidgetItem *pItem = NULL;
    CProjectWidget *pProject = NULL;

    QSettings settings(STRING_ORGANIZATION, STRING_APPLICATION_NAME);

    int iTabIndex, i;
    int iCount = settings.value("CProjectListWidget/Count", 0).toInt();

    addProject("", STRING_ALL_FOLDERS, false, settings.value(QString("CProjectListWidgetFiles/All") ).toStringList());
    pItem = this->item(this->count()-1);
    pProject = (CProjectWidget *)this->itemWidget(pItem);

    for (i=0;i<iCount;i++)
    {
       addProject( settings.value(QString("CProjectListWidgetPath/%1").arg(i) ).toString(),
                   settings.value(QString("CProjectListWidgetName/%1").arg(i) ).toString(),
                   true,
                   settings.value(QString("CProjectListWidgetFiles/%1").arg(i) ).toStringList());

       iTabIndex = settings.value( QString("CProjectListWidgetTabIndex/%1").arg(i), 0).toInt();
       pItem = this->item(this->count()-1);
       pProject = (CProjectWidget *)this->itemWidget(pItem);
       pProject->setTabulatorIndex(iTabIndex);
       pProject->loadSettings(&settings, i);
    }
}


void CProjectListWidget::saveSettings()
{
    int i;
    QListWidgetItem *pItem = NULL;
    CProjectWidget *pProject = NULL;
    QSettings settings(STRING_ORGANIZATION, STRING_APPLICATION_NAME);

    settings.setValue("CProjectListWidget/Count", this->count()-1);


    for (i=1;i<this->count();i++)
    {
        pItem = this->item(i);
        pProject = (CProjectWidget *)this->itemWidget(pItem);

        settings.setValue(QString("CProjectListWidgetPath/%1").arg(i-1), pProject->getProjectFolderPath() );
        settings.setValue(QString("CProjectListWidgetName/%1").arg(i-1), pProject->getProjectName() );

        settings.setValue(QString("CProjectListWidgetFiles/%1").arg(i-1), pProject->getRecentProjectFiles() );

        settings.setValue(QString("CProjectListWidgetTabIndex/%1").arg(i-1), pProject->getTabulatorIndex() );

        pProject->saveSettings(&settings, i-1);
    }

    pItem = this->item(0);
    pProject = (CProjectWidget *)this->itemWidget(pItem);
    settings.setValue(QString("CProjectListWidgetFiles/All"), pProject->getRecentProjectFiles() );
}

/**
 * @brief CProjectListWidget::setProjectListForFileName
 * @param strFileName
 * @return returns the folder project path
 * This function also changes the project list folder name for filename if required
 * and this function also changes the FolderTreeView by selectedProjectChanged() slot
 */
QString CProjectListWidget::setProjectListForFileName(QString strFileName)
{
    QListWidgetItem *pItem = currentItem();
    CProjectWidget *pProject = (CProjectWidget *)this->itemWidget(pItem);


    if (isFileNameValidForFolderProject(strFileName, pProject->getProjectFolderPath()))
    {
        selectedProjectChanged();
        return pProject->getProjectFolderPath();
    }

    int i, iToIndex = -1;
    QString strToFolderPath = "";

    for (i=0;i<this->count();i++)
    {
        pItem = this->item(i);
        pProject = (CProjectWidget *)this->itemWidget(pItem);

        if (isFileNameValidForFolderProject(strFileName, pProject->getProjectFolderPath()))
        {
            if (iToIndex == -1 || pProject->getProjectFolderPath().size() > strToFolderPath.size())
            {
                iToIndex = i;
                strToFolderPath = pProject->getProjectFolderPath();
            }
        }
    }

    this->setCurrentRow(iToIndex);
    return strToFolderPath;
}

bool CProjectListWidget::isFileNameValidForFolderProject(QString strFileName, QString strFolderPath)
{
    if (strFolderPath.isEmpty())
    {
        return true;
    }

    if (QDir::toNativeSeparators(strFolderPath) == QDir::toNativeSeparators(strFileName))
    {
        return true;
    }

    return QDir::toNativeSeparators(strFileName).startsWith( QDir::toNativeSeparators(strFolderPath) + QDir::separator() );
}


void CProjectListWidget::addProject(QString strProjectFolderPath,
                                    QString strProjectName, bool bButtonClose,
                                    QStringList listRecentClosedFiles, int iToIndex)
{
    int i;

    if (iToIndex == -1) {
        for (i=0;i<this->count();i++)
        {
            QListWidgetItem *pItemWidget = this->item(i);
            CProjectWidget *pProjectWidget = (CProjectWidget *)this->itemWidget(pItemWidget);
            if (pProjectWidget->getProjectFolderPath() == strProjectFolderPath)
            {
                return;
            }
        }
    }


    QListWidgetItem *pItem = new QListWidgetItem();
    if (iToIndex == -1) {
        iToIndex = this->count();
        this->addItem(pItem);
    }
    else {
        this->insertItem(iToIndex, pItem);
    }


    CProjectWidget *pProject = new CProjectWidget(m_pMainWindow, this, strProjectFolderPath, strProjectName, bButtonClose, listRecentClosedFiles);
    this->setItemWidget(pItem, pProject);
    this->setCurrentItem(pItem);

    refreshBackgroundColor();
}
/**
 * @brief CProjectListWidget::refreshBackgroundColor
 * refresh the background color of the list widget of projects
 * if the project folder exits, backgroundis white (normal)
 * if the project folder exists, background is red
 */
void CProjectListWidget::refreshBackgroundColor()
{
    int i;
    QListWidgetItem *pItem;
    CProjectWidget *pProject;

    for (i=1;i<this->count();i++)
    {
        pItem = this->item(i);
        pProject = (CProjectWidget *)this->itemWidget(pItem);

        QDir dir(pProject->getProjectFolderPath());
        if (dir.exists()) {
            pItem->setBackground(Qt::white);
        }
        else {
            pItem->setBackground(Qt::red);
        }
    }
}
/**
 * @brief CProjectListWidget::getSelectedItem
 * get pointer of the selected project
 * @return
 */
CProjectWidget *CProjectListWidget::getSelectedItem()
{
    QListWidgetItem *pItemWidget = this->currentItem();
    if (pItemWidget) {
        CProjectWidget *pProjectWidget = (CProjectWidget *)this->itemWidget(pItemWidget);
        return pProjectWidget;
    }
    return NULL;
}

/**
 * @brief CProjectListWidget::removeProject
 * remove the project folder from the list
 * @param strProjectFolderPath
 */
void CProjectListWidget::removeProject(QString strProjectFolderPath)
{
    int i;

    for (i=0;i<this->count();i++)
    {
        QListWidgetItem *pItemWidget = this->item(i);
        CProjectWidget *pProjectWidget = (CProjectWidget *)this->itemWidget(pItemWidget);

        if (pProjectWidget->getProjectFolderPath() == strProjectFolderPath)
        {
            pItemWidget = this->takeItem(i);
            delete pItemWidget;
            return;
        }
    }

    saveSettings();
}

/**
 * @brief CProjectListWidget::moveUp
 * move the widget up on the list
 * @param pWidget
 */

void CProjectListWidget::moveUp(CProjectWidget *pWidget)
{
    int i;
    for (i=2;i<this->count();i++)
    {
        QListWidgetItem *pItemWidget = this->item(i);
        CProjectWidget *pProjectWidget = (CProjectWidget *)this->itemWidget(pItemWidget);

        if (pProjectWidget->getProjectFolderPath() == pWidget->getProjectFolderPath())
        {
            pItemWidget = this->takeItem(i);
            addProject(pWidget->getProjectFolderPath(),
                       pWidget->getProjectName(), true,
                       pWidget->getRecentProjectFiles(), i-1);
            delete pItemWidget;
            delete pProjectWidget;
            return;
        }
    }
}
/**
 * @brief CProjectListWidget::moveDown
 * move the widget down on the list
 * @param pWidget
 */
void CProjectListWidget::moveDown(CProjectWidget *pWidget)
{
    int i;
    for (i=1;i<this->count()-1;i++)
    {
        QListWidgetItem *pItemWidget = this->item(i);
        CProjectWidget *pProjectWidget = (CProjectWidget *)this->itemWidget(pItemWidget);

        if (pProjectWidget->getProjectFolderPath() == pWidget->getProjectFolderPath())
        {
            pItemWidget = this->takeItem(i);
            addProject(pWidget->getProjectFolderPath(),
                       pWidget->getProjectName(), true,
                       pWidget->getRecentProjectFiles(), i+1);
            delete pItemWidget;
            delete pProjectWidget;
            return;
        }
    }
}

/**
 * @brief CProjectListWidget::isProjectFolderLess
 * when sorting the list, this function decides which folder is above others
 * @param strFolder0
 * @param strFolder1
 * @param strProjectName0
 * @param strProjectName1
 * @return
 */
bool CProjectListWidget::isProjectFolderLess(QString strFolder0, QString strFolder1, QString strProjectName0, QString strProjectName1)
{
    QDir dir0(strFolder0);
    QDir dir1(strFolder1);
    if (dir0.exists() == dir1.exists()) {
        if (strProjectName0 < strProjectName1) {
            return true;
        }
        else {
            return false;
        }

    }
    else {
        if (dir0.exists()) {
            return true;
        }
        else {
            return false;
        }
    }
}

/**
 * @brief CProjectListWidget::sortTheList
 * sort the list
 */

void CProjectListWidget::sortTheList()
{
    QListWidgetItem *pItemWidget0;
    QListWidgetItem *pItemWidget1;
    CProjectWidget *pProjectWidget0;
    CProjectWidget *pProjectWidget1;
    int i;
    bool bDone;
    int iCount = getProjectCount();
    while (1)
    {
        bDone = false;
        for (i=1;i<iCount-1;i++) {
            pItemWidget0 = this->item(i);
            pProjectWidget0 = (CProjectWidget *)this->itemWidget(pItemWidget0);
            pItemWidget1 = this->item(i+1);
            pProjectWidget1 = (CProjectWidget *)this->itemWidget(pItemWidget1);

            if (isProjectFolderLess(pProjectWidget1->getProjectFolderPath(), pProjectWidget0->getProjectFolderPath(),
                                    pProjectWidget1->getProjectName(), pProjectWidget0->getProjectName())) {
                moveDown(pProjectWidget0);
                bDone = true;
            }
        }

        if (bDone == false) {
            break;
        }

        // this counter doing the the list may actually speed things up if the list is very mixed up
        bDone = false;
        for (i=iCount-2;i>=1;i--) {
            pItemWidget0 = this->item(i);
            pProjectWidget0 = (CProjectWidget *)this->itemWidget(pItemWidget0);
            pItemWidget1 = this->item(i+1);
            pProjectWidget1 = (CProjectWidget *)this->itemWidget(pItemWidget1);

            if (isProjectFolderLess(pProjectWidget1->getProjectFolderPath(), pProjectWidget0->getProjectFolderPath(),
                                    pProjectWidget1->getProjectName(), pProjectWidget0->getProjectName())) {
                moveUp(pProjectWidget1);
                bDone = true;
            }
        }

        if (bDone == false) {
            break;
        }
    }
}

int CProjectListWidget::getProjectCount()
{
    return this->count();
}

int CProjectListWidget::getProjectIndex(CProjectWidget *pWidget)
{
    int i;
    for (i=0;i<this->count();i++)
    {
        QListWidgetItem *pItemWidget = this->item(i);
        CProjectWidget *pProjectWidget = (CProjectWidget *)this->itemWidget(pItemWidget);

        if (pProjectWidget->getProjectFolderPath() == pWidget->getProjectFolderPath())
        {
            return i;
        }
    }
    return -1;
}


void CProjectListWidget::selectedProjectChanged()
{
    QListWidgetItem *pItem = currentItem();
    CProjectWidget *pProject = (CProjectWidget *)this->itemWidget(pItem);

    m_pMainWindow->getEditorTabWidget()->setWidgetsForFolder( pProject->getProjectFolderPath(), pProject->getLastOpenIndex() );

    if (pProject->getProjectFolderPath().isEmpty() == false)
    {
        m_pMainWindow->getFolderTreeView()->setToPath( pProject->getProjectFolderPath() );
    }

    if ( pProject->getTabulatorIndex() != m_pMainWindow->getToolBarWidget()->getTabulatorIndex()) {

        m_pMainWindow->getToolBarWidget()->setTabulatorIndex( pProject->getTabulatorIndex() );
    }
}

void CProjectListWidget::onRunProject()
{
    if (this->currentIndex().row() > 0) {

        QListWidgetItem *pItem = currentItem();
        CProjectWidget *pProject = (CProjectWidget *)this->itemWidget(pItem);
        pProject->onRunProject();
    }
}

void CProjectListWidget::onFolderRename(QString strFrom, QString strTo)
{
    int i;
    for (i=1;i<this->count();i++)
    {
        QListWidgetItem *pItemWidget = this->item(i);
        CProjectWidget *pProjectWidget = (CProjectWidget *)this->itemWidget(pItemWidget);
        pProjectWidget->onFolderRename(strFrom, strTo);
    }
}


void CProjectListWidget::closeFile(QString strFullFilePath)
{
    int i;
    for (i=0;i<this->count();i++)
    {
        QListWidgetItem *pItemWidget = this->item(i);
        CProjectWidget *pProjectWidget = (CProjectWidget *)this->itemWidget(pItemWidget);
        pProjectWidget->closeFile(strFullFilePath);
    }
}

void CProjectListWidget::openFile(QString strFullFilePath)
{
    int i;
    for (i=0;i<this->count();i++)
    {
        QListWidgetItem *pItemWidget = this->item(i);
        CProjectWidget *pProjectWidget = (CProjectWidget *)this->itemWidget(pItemWidget);
        pProjectWidget->openFile(strFullFilePath);
    }
}
