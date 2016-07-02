#include <QHeaderView>
#include <QFile>
#include <QDir>
#include <QStandardItem>
#include <QTextStream>
#include <QDebug>

#include "application.h"
#include "cmainwindow.h"
#include "cdialogfind.h"
#include "cfindfromfilestreeview.h"
#include "cfoldertreeview.h"
#include "ceditortabwidget.h"

CFindFromFilesTreeView::CFindFromFilesTreeView(CMainWindow *pMainWindow, QWidget *parent) :
    QTreeView(parent),
    m_pMainWindow(pMainWindow)
{
    m_pModel = new QStandardItemModel(this);
    QStringList horizontalHeader;
    horizontalHeader.append(STRING_FILE);
    horizontalHeader.append(STRING_LINE);
    horizontalHeader.append(STRING_TEXT);
    m_pModel->setHorizontalHeaderLabels(horizontalHeader);


    setModel(m_pModel);
}


void CFindFromFilesTreeView::clear()
{
    m_listSearchedDirs.clear();
    m_pModel->removeRows(0, m_pModel->rowCount());
}

void CFindFromFilesTreeView::add(QString strFile, QString strLine, QString strText)
{
    int iLineIndex = m_pModel->rowCount();

    QStandardItem *item00 = new QStandardItem(QDir::toNativeSeparators(strFile));
    QStandardItem *item01 = new QStandardItem(strLine);
    QStandardItem *item02 = new QStandardItem(strText);

    item00->setEditable(false);
    item01->setEditable(false);
    item02->setEditable(false);

    m_pModel->setItem(iLineIndex, 0, item00);
    m_pModel->setItem(iLineIndex, 1, item01);
    m_pModel->setItem(iLineIndex, 2, item02);
}

void CFindFromFilesTreeView::searchFromFile(QString strFile, const QString &strText)
{
    int iLineIndex = 0;
    QString strLine;
    QFile file(strFile);
    if(!file.open(QIODevice::ReadOnly)) {
        return;
    }
    QTextStream in(&file);

    while(!in.atEnd())
    {
        strLine = in.readLine();
        iLineIndex++;

        if (strLine.indexOf(strText, 0, m_pMainWindow->getDialogFind()->getCaseSensitivity()) != -1)
        {
            add(strFile, QString("%1").arg(iLineIndex), strLine);
        }
    }

    file.close();

}

void CFindFromFilesTreeView::searchFromDir(const QString &dirName, const QString &dirOriginalName)
{
    if (!QDir::toNativeSeparators(dirName).startsWith(QDir::toNativeSeparators(dirOriginalName)))
    {
        return;
    }

    QString strText = m_pMainWindow->getDialogFind()->getFindText();
    QDir dir(dirName);
    m_listSearchedDirs.append(dirName);

    QDir::Filters filter = QDir::Files;

    if (m_pMainWindow->getDialogFind()->getFindIncludeHidden())
    {
        filter = QDir::Files | QDir::Hidden;
    }

    QStringList nameFilter;
    nameFilter << m_pMainWindow->getDialogFind()->getFindFileTypes();
    QFileInfoList list = dir.entryInfoList(nameFilter, filter);
    int i;

    for (i=0;i<list.count();i++)
    {
        QFileInfo info = list.at(i);
        searchFromFile(info.absoluteFilePath(), strText);
    }

    if (m_pMainWindow->getDialogFind()->getFindSubDirs())
    {
        filter = QDir::Dirs;
        if (m_pMainWindow->getDialogFind()->getFindIncludeHidden())
        {
            filter = QDir::Dirs | QDir::Hidden;
        }

        list = dir.entryInfoList(filter);
        for (i=0;i<list.count();i++)
        {
            QFileInfo info = list.at(i);

            if (info.fileName() != "." && info.fileName() != "..")
            {
                QString strDirName = info.absoluteFilePath();

                if (!m_listSearchedDirs.contains(strDirName))
                {
                    searchFromDir(strDirName, dirOriginalName);
                }
            }
        }
    }
}

int CFindFromFilesTreeView::searchFromFiles()
{
    clear();

    m_strSearchText = m_pMainWindow->getDialogFind()->getFindText();

    QString strDirName =  m_pMainWindow->getDialogFind()->getFindDirName();
    searchFromDir(strDirName, strDirName);

    return m_pModel->rowCount();
}

void CFindFromFilesTreeView::currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    QString strFileName;
    QString strFileLine;
    strFileName = current.sibling(current.row(), 0).data().toString();
    strFileLine = current.sibling(current.row(), 1).data().toString();

    QTreeView::currentChanged(current, previous);

    if (m_pMainWindow->getFolderTreeView()) {

        m_pMainWindow->getFolderTreeView()->setToPath(strFileName);
        emit m_pMainWindow->getFolderTreeView()->openFile(strFileName);

        m_pMainWindow->getEditorTabWidget()->setCursorPosition(strFileLine.toInt()-1, m_strSearchText);
        // This is a fix in case path of files treeview is switch to folder (by folder project change)
        m_pMainWindow->getFolderTreeView()->setToPath(strFileName);
        m_pMainWindow->getFolderTreeView()->openFile(strFileName);
    }
}
