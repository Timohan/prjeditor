#include <QDebug>
#include <QDirModel>
#include <QMessageBox>
#include <QHeaderView>
#include <QAction>
#include <QMenu>
#include <QFileSystemWatcher>
#include "application.h"
#include "cfoldertreeview.h"
#include "cfoldersortfilter.h"
#include "cprojectlistwidget.h"
#include "cmainwindow.h"
#include "cdialogquerymessage.h"

CFolderTreeView::CFolderTreeView(CMainWindow *parent) :
    QTreeView (parent),
    m_modelFileSystem(NULL),
    m_pProjectListWidget(NULL),
    m_dialogCreate(NULL),
    m_pFolderWatcher(NULL),
    m_pMainWindow(parent)
{
    int i;

    m_modelFileSystem = new QDirModel(this);
    m_modelFileSystem->setSorting(QDir::DirsFirst |
                      QDir::IgnoreCase |
                      QDir::Name);
    setModel(m_modelFileSystem);

    m_pFolderWatcher = new QFileSystemWatcher(this);
    connect(m_pFolderWatcher, SIGNAL(directoryChanged(const QString &)), this, SLOT(directoryChanged(const QString &)));
    connect(m_pFolderWatcher, SIGNAL(fileChanged(const QString &)), this, SLOT(fileChanged(const QString &)));
    for (i=1;i<m_modelFileSystem->columnCount();i++)
    {
        hideColumn(i);
    }

    this->header()->setVisible(false);

    connect(this, SIGNAL(collapsed(const QModelIndex &)), this, SLOT(onCollapsed(const QModelIndex &)));
    connect(this, SIGNAL(expanded(const QModelIndex &)), this, SLOT(onExpanded(const QModelIndex &)));
    connect(this, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(onDoubleClicked(const QModelIndex &)));

    setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)),
        this, SLOT(ShowContextMenu(const QPoint&)));

    m_dialogCreate = new CDialogQueryMessage(this);
}

void CFolderTreeView::onDoubleClicked(const QModelIndex & index)
{

    if (!m_modelFileSystem->isDir(index))
    {
        QVariant data = m_modelFileSystem->data(index);
        emit openFile(m_modelFileSystem->filePath(index));
    }
}


void CFolderTreeView::directoryChanged(const QString &path)
{
    QModelIndex index = m_modelFileSystem->index(path);
    m_modelFileSystem->refresh(index);

    QFileInfo info(path);
    if (info.isDir())
    {
        QDir dir(path);
        QFileInfoList list = dir.entryInfoList(QDir::NoFilter, QDir::DirsFirst |
                          QDir::IgnoreCase |
                          QDir::Name);

        int i;

        for (i=0;i<list.size();i++) {
            directoryChangedNew(list[i].absoluteFilePath());
        }
    }
    m_pMainWindow->getProjectListWidget()->refreshBackgroundColor();
}

void CFolderTreeView::directoryChangedNew(const QString &path)
{
    QModelIndex index = m_modelFileSystem->index(path);
    m_modelFileSystem->refresh(index);
    m_pMainWindow->getProjectListWidget()->refreshBackgroundColor();
}

void CFolderTreeView::fileChanged(const QString &path)
{
    QModelIndex index = m_modelFileSystem->index(path);
    m_modelFileSystem->refresh(index);
}

void CFolderTreeView::onCollapsed(const QModelIndex &)
{
    /*
    QVariant data = m_modelFileSystem->data(index);
    qDebug() << "void CFolderTreeView::onCollapsed(const QModelIndex &index)" << data;
    qDebug() << m_modelFileSystem->filePath(index);
    qDebug() << m_modelFileSystem->fileName(index);
    m_pFolderWatcher->removePath(m_modelFileSystem->filePath(index));
    */
}

void CFolderTreeView::addEmptySubfoldersOnFolderWatch(QString strFolder)
{
    int i;
    QDir dir(strFolder);
    QStringList entries = dir.entryList(QDir::AllDirs);

    for (i=0;i<entries.size();i++)
    {
        m_pFolderWatcher->addPath(strFolder + entries.at(i));
    }
}

void CFolderTreeView::onExpanded(const QModelIndex &index)
{
    QVariant data = m_modelFileSystem->data(index);
    QString strFolder = m_modelFileSystem->filePath(index);
    m_pFolderWatcher->addPath(strFolder);
    addEmptySubfoldersOnFolderWatch(strFolder);
}


void CFolderTreeView::setProjectListWidget(CProjectListWidget *pProjectListWidget)
{
    m_pProjectListWidget = pProjectListWidget;
}


void CFolderTreeView::ShowContextMenu(const QPoint&pos)
{
    QModelIndex index = this->currentIndex();
    if (index.isValid()) {
        QString strFolderPath = m_modelFileSystem->filePath(index);
        QString strProjectName = m_modelFileSystem->fileName(index);
        QString strTemp;

        QPoint globalPos = this->mapToGlobal(pos);
        QMenu myMenu;

        myMenu.addAction(STRING_FIND_IN_FILES_FROM_THIS_PATH);
        if (m_modelFileSystem->isDir(index))
        {
            myMenu.addAction(STRING_PROJECT_ADD_FOLDER);
            myMenu.addAction(STRING_PROJECT_CREATE_FOLDER);
            myMenu.addAction(STRING_PROJECT_CREATE_FILE);
            myMenu.addAction(STRING_RENAME_FOLDER);
            myMenu.addAction(STRING_REFRESH);
            myMenu.addAction(STRING_DELETE_FOLDER);
        }
        else
        {
            myMenu.addAction(STRING_PROJECT_ADD_FOLDER);
            myMenu.addAction(STRING_PROJECT_CREATE_FOLDER);
            myMenu.addAction(STRING_PROJECT_CREATE_FILE);
            myMenu.addAction(STRING_PROJECT_OPEN_FILE);
            myMenu.addAction(STRING_RENAME_FILE);
            myMenu.addAction(STRING_DELETE_FILE);
        }

        QAction* selectedItem = myMenu.exec(globalPos);
        if (selectedItem)
        {
            if (selectedItem->text() == STRING_PROJECT_ADD_FOLDER)
            {
                if (m_modelFileSystem->isDir(index))
                {
                    m_pProjectListWidget->addProject(strFolderPath, strProjectName);
                }
                else
                {
                    int iLastIndex;
                    int iLastIndex1 = strFolderPath.lastIndexOf("/");
                    int iLastIndex0 = strFolderPath.lastIndexOf("\\");

                    if (iLastIndex0 == -1 || iLastIndex0 < iLastIndex1)
                    {
                        iLastIndex = iLastIndex1;
                    }
                    else
                    {
                        iLastIndex = iLastIndex0;
                    }

                    strFolderPath.resize(iLastIndex);

                    iLastIndex1 = strFolderPath.lastIndexOf("/");
                    iLastIndex0 = strFolderPath.lastIndexOf("\\");

                    if (iLastIndex0 == -1 || iLastIndex0 < iLastIndex1)
                    {
                        iLastIndex = iLastIndex1;
                    }
                    else
                    {
                        iLastIndex = iLastIndex0;
                    }

                    strProjectName = strFolderPath.right(strFolderPath.size() - iLastIndex - 1);

                    m_pProjectListWidget->addProject(strFolderPath, strProjectName);
                }
            }
            else if (selectedItem->text() == STRING_REFRESH) {
                refresh(strFolderPath);
            }
            else if (selectedItem->text() == STRING_FIND_IN_FILES_FROM_THIS_PATH) {

                if (!m_modelFileSystem->isDir(index)) {
                    strFolderPath.resize(strFolderPath.size()-strProjectName.size());
                }

                m_pMainWindow->onOpenDialogFindInFiles( strFolderPath );
            }
            else if (selectedItem->text() == STRING_PROJECT_OPEN_FILE)
            {
                onDoubleClicked(index);
            }
            else if (selectedItem->text() == STRING_PROJECT_CREATE_FOLDER)
            {
                m_dialogCreate->setDialog(STRING_PROJECT_CREATE_FOLDER, STRING_FOLDER_NAME);
                if (m_dialogCreate->exec() == 1 && m_dialogCreate->getAnswer().isEmpty() == false)
                {
                    if (!m_modelFileSystem->isDir(index))
                    {
                        strFolderPath.resize(strFolderPath.size()-strProjectName.size());
                    }

                    if (strFolderPath.endsWith("/") || strFolderPath.endsWith("\\"))
                    {
                        strTemp = QDir::toNativeSeparators(strFolderPath + m_dialogCreate->getAnswer());
                    }
                    else
                    {
                        strTemp = QDir::toNativeSeparators(strFolderPath + "/" + m_dialogCreate->getAnswer());
                    }
                    QDir nDir;
                    if (!nDir.exists(strTemp))
                    {
                        if (nDir.mkpath(strTemp) == false)
                        {
                            QMessageBox::warning(this, STRING_ERROR, STRING_ERROR_CREATE_FOLDER_FAIL);
                        }
                    }
                    else
                    {
                        QMessageBox::warning(this, STRING_ERROR, STRING_FOLDER_ALREADY_EXISTS);
                    }


                }
            }
            else if (selectedItem->text() == STRING_PROJECT_CREATE_FILE)
            {
                m_dialogCreate->setDialog(STRING_PROJECT_CREATE_FILE, STRING_FILENAME);
                if (m_dialogCreate->exec() == 1 && m_dialogCreate->getAnswer().isEmpty() == false)
                {
                    if (!m_modelFileSystem->isDir(index))
                    {
                        strFolderPath.resize(strFolderPath.size()-strProjectName.size());
                    }

                    if (strFolderPath.endsWith("/") || strFolderPath.endsWith("\\"))
                    {
                        strTemp = QDir::toNativeSeparators(strFolderPath + m_dialogCreate->getAnswer());
                    }
                    else
                    {
                        strTemp = QDir::toNativeSeparators(strFolderPath + "/" + m_dialogCreate->getAnswer());
                    }
                    QFile nFile( strTemp );

                    if (nFile.exists() == false)
                    {
                        if (nFile.open(QFile::WriteOnly))
                        {
                            nFile.close();
                        }
                        else
                        {
                            QMessageBox::warning(this, STRING_ERROR, STRING_ERROR_CREATE_FILE_FAIL);
                        }
                    }
                    else
                    {
                        QMessageBox::warning(this, STRING_ERROR, STRING_FILE_ALREADY_EXISTS);
                    }
                }
            }
            else if (selectedItem->text() == STRING_RENAME_FOLDER)
            {
                m_dialogCreate->setDialog(STRING_RENAME_FOLDER, STRING_FOLDER_NAME, QDir::toNativeSeparators(strFolderPath));

                if (m_dialogCreate->exec() == 1 && m_dialogCreate->getAnswer().isEmpty() == false)
                {
                    QDir nRenameDir(strFolderPath);
                    if (nRenameDir.rename(strFolderPath, m_dialogCreate->getAnswer()))
                    {
                        m_pMainWindow->onRename(strFolderPath, m_dialogCreate->getAnswer(), false);
                    }
                    else
                    {
                        QMessageBox::warning(this, STRING_ERROR, STRING_ERROR_RENAME_FOLDER);
                    }
                }
            }
            else if (selectedItem->text() == STRING_RENAME_FILE)
            {
                m_dialogCreate->setDialog(STRING_RENAME_FILE, STRING_FILENAME, QDir::toNativeSeparators(strFolderPath));

                if (m_dialogCreate->exec() == 1 && m_dialogCreate->getAnswer().isEmpty() == false)
                {
                    QFile nRenameFile( strFolderPath );
                    if (nRenameFile.rename(m_dialogCreate->getAnswer()))
                    {
                        m_pMainWindow->onRename(strFolderPath, m_dialogCreate->getAnswer(), true);
                    }
                    else
                    {
                        QMessageBox::warning(this, STRING_ERROR, STRING_ERROR_RENAME_FILE);
                    }
                }
            }
            else if (selectedItem->text() == STRING_DELETE_FILE)
            {
                if (QMessageBox::question(m_pMainWindow, STRING_CONFIRM_DELETE,
                                      QString(STRING_ARE_YOU_SURE_YOU_WANT_TO_DELETE_FILE).arg(QDir::toNativeSeparators(strFolderPath))) == QMessageBox::Yes) {

                    QFile fileToDelete( strFolderPath );
                    fileToDelete.remove();
                }
            }
            else if (selectedItem->text() == STRING_DELETE_FOLDER) {

                if (QMessageBox::question(m_pMainWindow, STRING_CONFIRM_DELETE,
                                      QString(STRING_ARE_YOU_SURE_YOU_WANT_TO_DELETE_FOLDER).arg(QDir::toNativeSeparators(strFolderPath))) == QMessageBox::Yes) {

                    QDir dirToDelete( strFolderPath );
                    dirToDelete.removeRecursively();
                }
            }
        }
    }
}

void CFolderTreeView::refresh(QString strPath)
{
    int i;
    QString strFileList = runCommandLine("ls " + strPath);
    QStringList list;
    list = strFileList.split("\n");

    QModelIndex pParentIndex = m_modelFileSystem->index(strPath);

    if (pParentIndex.isValid()) {

        for (i=0;i<list.count();i++) {
            QModelIndex index = m_modelFileSystem->index(strPath + "/" + list[i]);
            m_modelFileSystem->refresh(index);
        }
    }
}
/**
 * @brief CFolderTreeView::runCommandLine
 * run the command line command
 * @param strCmdLine, command line
 * @return returns the command line's result
 */
QString CFolderTreeView::runCommandLine(QString strCmdLine)
{
    char buf[ 1024 ];
    QString rv = "";

    QByteArray ba = strCmdLine.toLocal8Bit();
    const char *c_strCmdLine = ba.data();

    FILE* pfile = popen(c_strCmdLine, "r");
    while( fgets( buf, 1024,  pfile ) ) {
        rv += buf;
    }
    pclose( pfile );

    return rv;
}

void CFolderTreeView::setToPath(QString strRootPath)
{
    if (!strRootPath.isEmpty())
    {
        QModelIndex indexOriginal = this->currentIndex();
        QString strFolderPath = m_modelFileSystem->filePath(indexOriginal);
        QString strFileName = m_modelFileSystem->fileName(indexOriginal);
        QString strFullFilePathName;

        if (strFolderPath.isEmpty() == false) {

            if (m_modelFileSystem->isDir(indexOriginal)) {
                strFullFilePathName = strFolderPath;
            }
            else {
                strFullFilePathName = strFolderPath + QDir::separator() + strFileName;
            }
        }

        if (strFullFilePathName != strRootPath) {
            QModelIndex index = m_modelFileSystem->index(strRootPath);
            if (index != indexOriginal) {

                expand(index);
                scrollTo(index);
                setCurrentIndex(index);
                resizeColumnToContents(0);
            }
        }
    }
}
