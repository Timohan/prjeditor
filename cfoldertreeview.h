#ifndef CFOLDERTREEVIEW_H
#define CFOLDERTREEVIEW_H

#include <QTreeView>
class QDirModel;
class CDialogQueryMessage;
class CProjectListWidget;
class QFileSystemWatcher;
class CMainWindow;


class CFolderTreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit CFolderTreeView(CMainWindow *parent = 0);
    
    void setToPath(QString strToPath);
    void setProjectListWidget(CProjectListWidget *pProjectListWidget);

    void refresh(QString strPath);
signals:
    
public slots:
    void ShowContextMenu(const QPoint&);
    void directoryChanged(const QString &path);
    void fileChanged(const QString &path);
    void onCollapsed(const QModelIndex & index);
    void onExpanded(const QModelIndex & index);
    void onDoubleClicked(const QModelIndex & index);

signals:
    void openFile(QString strFileName);

private:
    QDirModel *m_modelFileSystem;
    void addEmptySubfoldersOnFolderWatch(QString strFolder);
    void directoryChangedNew(const QString &path);
    QString runCommandLine(QString strCmdLine);

    CProjectListWidget *m_pProjectListWidget;

    CDialogQueryMessage *m_dialogCreate;
    QFileSystemWatcher *m_pFolderWatcher;
    /**
     * @brief m_pMainWindow
     * pointer to main window
     */
    CMainWindow *m_pMainWindow;
};

#endif // CFOLDERTREEVIEW_H
