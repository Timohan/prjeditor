#include <QLabel>
#include <QDebug>
#include <QMenu>
#include <QDir>
#include <QPainter>
#include "cmainwindow.h"
#include "cprojectwidget.h"
#include "cbutton.h"
#include "application.h"
#include "cprojectlistwidget.h"
#include "cdialogquerymessage.h"
#include "cdialogrunprgram.h"
#include "cfoldertreeview.h"

CProjectWidget::CProjectWidget(CMainWindow *pMainWindow, CProjectListWidget *parent, QString strProjectFolderPath,
                               QString strProjectName, bool bButtonClose, QStringList listRecentClosedFiles, int iTabulatorIndex) :
    QWidget(parent),
    m_buttonClose(NULL),
    m_parentProjectListWidget(parent),
    m_pMainWindow(pMainWindow),
    m_listRecentProjectFiles(listRecentClosedFiles),
    m_iLastOpenIndex(-1),
    m_iTabulatorIndex(iTabulatorIndex)
{
    m_labelProjectName = new QLabel(strProjectName, this);
    m_dialogRunProgram = new CDialogRunPrgram(this, this, parent);

    if (bButtonClose) {
        m_buttonClose = new CButton(this, STRING_PROJECT_CLOSE, "project/close.png");
        m_buttonClose->setFlat(true);
        connect(m_buttonClose, SIGNAL(clicked()), this, SLOT(onCloseClicked()));
    }
    setProjectFolderPath(strProjectFolderPath);

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)),
        this, SLOT(ShowContextMenu(const QPoint&)));
}



CProjectWidget::~CProjectWidget()
{
    if (m_buttonClose)
    {
        delete m_buttonClose;
        m_buttonClose = NULL;
    }

    delete m_labelProjectName;
    m_labelProjectName = NULL;

    if (m_dialogRunProgram) {

        delete m_dialogRunProgram;
        m_dialogRunProgram = NULL;
    }
}

int CProjectWidget::getTabulatorIndex() {

    return m_iTabulatorIndex;
}

void CProjectWidget::setTabulatorIndex(int iTabulatorIndex) {

    m_iTabulatorIndex = iTabulatorIndex;
}

int CProjectWidget::getLastOpenIndex()
{
    return m_iLastOpenIndex;
}

void CProjectWidget::setLastOpenIndex(int iIndex)
{
    m_iLastOpenIndex = iIndex;
}

void CProjectWidget::saveSettings(QSettings *pSettings, int iIndex)
{
    m_dialogRunProgram->saveSettings(pSettings, iIndex);
}

void CProjectWidget::loadSettings(QSettings *pSettings, int iIndex)
{
    m_dialogRunProgram->loadSettings(pSettings, iIndex);
}

QString CProjectWidget::getProjectFolderPath() const
{
    return m_strProjectFolderPath;
}

void CProjectWidget::setProjectFolderPath(QString strProjectFolderPath)
{
    m_strProjectFolderPath = QDir::toNativeSeparators(strProjectFolderPath);
}

QString CProjectWidget::getProjectName() const
{
    return m_labelProjectName->text();
}

void CProjectWidget::setProjectName(QString strProjectName)
{
    m_labelProjectName->setText(strProjectName);
}

void CProjectWidget::resizeEvent(QResizeEvent *event)
{
    if (event)
    {
        QWidget::resizeEvent(event);
    }

    QFontMetrics fontMetrics(m_labelProjectName->font());
    int iTextHeight = fontMetrics.height();
    m_labelProjectName->setGeometry(5, 0, this->width()-iTextHeight, iTextHeight);

    if (m_buttonClose)
    {
        m_buttonClose->setGeometry(this->width() - m_labelProjectName->height(),
                                   0,
                                   m_labelProjectName->height(),
                                   m_labelProjectName->height());
    }
}

void CProjectWidget::onCloseClicked()
{
    m_parentProjectListWidget->removeProject(m_strProjectFolderPath);
}

bool CProjectWidget::onFolderRename(QString strFrom, QString strTo)
{

    strFrom = QDir::toNativeSeparators(strFrom);
    strTo = QDir::toNativeSeparators(strTo);

    QString strTempDirName = strFrom +  QDir::separator();

    if (getProjectFolderPath().startsWith(strTempDirName) ||
        getProjectFolderPath() == strFrom)
    {
        if (getProjectFolderPath() == strFrom)
        {
            strTempDirName = strTo.right( strTo.size() - strTo.lastIndexOf(QDir::separator()) - 1);

            setProjectFolderPath(strTo);
            setProjectName(strTempDirName);
        }
        else
        {
            strTempDirName = getProjectFolderPath().right( getProjectFolderPath().size() - strFrom.size() );
            setProjectFolderPath(strTo + strTempDirName);
        }

        return true;
    }

    return false;
}

void CProjectWidget::closeFile(QString strFullFilePath)
{
    int i;
    strFullFilePath = QDir::toNativeSeparators(strFullFilePath);

    for (i=0;i<m_listRecentProjectFiles.size();i++) {

        if (m_listRecentProjectFiles[i] == strFullFilePath) {
            return;
        }
    }

    if (CProjectListWidget::isFileNameValidForFolderProject(strFullFilePath, getProjectFolderPath())) {

        m_listRecentProjectFiles.insert(0, strFullFilePath);
    }

}

void CProjectWidget::openFile(QString strFullFilePath)
{
    int i;

    strFullFilePath = QDir::toNativeSeparators(strFullFilePath);

    if (CProjectListWidget::isFileNameValidForFolderProject(strFullFilePath, getProjectFolderPath())) {

        for (i=0;i<m_listRecentProjectFiles.size();i++) {

            if (m_listRecentProjectFiles[i] == strFullFilePath) {
                m_listRecentProjectFiles.removeAt(i);
                return;
            }
        }
    }
}


void CProjectWidget::ShowContextMenu(const QPoint&pos)
{
    int i;
    int iIndex = m_parentProjectListWidget->getProjectIndex(this);
    int iCount = m_parentProjectListWidget->getProjectCount();
    QPoint globalPos = this->mapToGlobal(pos);
    QMenu myMenu;
    myMenu.addAction(STRING_REFRESH);
    myMenu.addAction(STRING_CHANGE_NAME);
    myMenu.addAction(STRING_FIND_IN_FILES_FROM_THIS_PATH);

    if (iIndex < iCount-1 && iIndex != 0) {
        myMenu.addAction(STRING_MOVE_DOWN);
    }

    if (iIndex >= 2) {
        myMenu.addAction(STRING_MOVE_UP);
    }

    for (i=0;i<m_listRecentProjectFiles.size() && i<10;i++) {

        myMenu.addAction(m_listRecentProjectFiles.at(i));
    }

    myMenu.addAction(STRING_SORT_THE_LIST);

    QAction* selectedItem = myMenu.exec(globalPos);
    if (selectedItem)
    {
        if (selectedItem->text().isEmpty() == false) {
            if (selectedItem->text() == STRING_SORT_THE_LIST) {
                m_parentProjectListWidget->sortTheList();
            }
            else if (selectedItem->text() == STRING_REFRESH) {
                m_pMainWindow->getFolderTreeView()->refresh( getProjectFolderPath() );
                m_parentProjectListWidget->refreshBackgroundColor();
            }
            else if (selectedItem->text() == STRING_CHANGE_NAME) {
                CDialogQueryMessage dialog(this);
                dialog.setDialog(STRING_CHANGE_NAME_PROJECT_NAME, this->getProjectName(), this->getProjectName());
                if (dialog.exec() == 1)
                {
                    if (dialog.getAnswer().isEmpty() == false) {
                        setProjectName(dialog.getAnswer());
                    }
                }
                m_parentProjectListWidget->refreshBackgroundColor();
            }
            else if (selectedItem->text() == STRING_MOVE_DOWN) {
                m_parentProjectListWidget->moveDown(this);
                m_parentProjectListWidget->refreshBackgroundColor();
            }
            else if (selectedItem->text() == STRING_MOVE_UP) {
                m_parentProjectListWidget->moveUp(this);
                m_parentProjectListWidget->refreshBackgroundColor();
            }
            else if (selectedItem->text() == STRING_FIND_IN_FILES_FROM_THIS_PATH) {
                m_pMainWindow->onOpenDialogFindInFiles( getProjectFolderPath() );
            }
            else {
                m_pMainWindow->openNewFile( selectedItem->text() );
                m_parentProjectListWidget->refreshBackgroundColor();
            }
        }
    }
    else
    {
            // nothing was chosen
        qDebug() << "Selected nothing";
    }
}

QStringList CProjectWidget::getRecentProjectFiles()
{
    return m_listRecentProjectFiles;
}

void CProjectWidget::onRunProject()
{
    m_dialogRunProgram->exec();
}

