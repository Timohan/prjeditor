#include <windows.h>
#include "cfolderwatcher.h"

CFolderWatcher::CFolderWatcher(QObject *parent) :
    QObject(parent)
{
}


bool CFolderWatcher::watchFolder(QString strFolder)
{
//    QFileSystemWatcher *watch_files = new QFileSystemWatcher(strFolder);
//    connect(watch_files, SIGNAL(directoryChanged(QString)), this, SLOT(research_folders()));

    /*
    OVERLAPPED	m_Overlapped;
    ::ZeroMemory(&m_Overlapped, sizeof(m_Overlapped));
        m_Overlapped.hEvent = this;

        DWORD dwBytes=0;

        BOOL success = ::ReadDirectoryChangesW(
            m_hDirectory,
            &m_Buffer[0],
            m_Buffer.size(),
            FALSE, // monitor children?
            FILE_NOTIFY_CHANGE_LAST_WRITE
             | FILE_NOTIFY_CHANGE_CREATION
             | FILE_NOTIFY_CHANGE_FILE_NAME,
            &dwBytes,
            &m_Overlapped,
            &NotificationCompletion);
        */
}
