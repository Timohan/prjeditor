#ifndef CPROJECTLISTWIDGET_H
#define CPROJECTLISTWIDGET_H

#include <QListWidget>
#include <QList>

class CFolderTreeView;
class CProjectWidget;
class CEditorWidget;
class CMainWindow;

class CProjectListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit CProjectListWidget(CMainWindow *parent);
    ~CProjectListWidget();

    void addProject(QString strProjectFolderPath, QString strProjectName, bool bButtonClose = true, QStringList listRecentClosedFiles = QStringList(), int iToIndex = -1);
    void removeProject(QString strProjectFolderPath);

    QString setProjectListForFileName(QString strFileName);
    static bool isFileNameValidForFolderProject(QString strFileName, QString strFolderPath);

    void closeFile(QString strFullFilePath);
    void openFile(QString strFullFilePath);

    void onFolderRename(QString strFrom, QString strTo);

    void moveDown(CProjectWidget *pWidget);
    void moveUp(CProjectWidget *pWidget);
    int getProjectCount();
    int getProjectIndex(CProjectWidget *pWidget);

    void saveSettings();
    void loadSettings();

    void onRunProject();

    CProjectWidget *getSelectedItem();

    void refreshBackgroundColor();
    void sortTheList();
signals:
    
public slots:
    void selectedProjectChanged();

private:
    CMainWindow *m_pMainWindow;
    bool isProjectFolderLess(QString strFolder0, QString strFolder1, QString strProjectName0, QString strProjectName1);

};

#endif // CPROJECTLISTWIDGET_H
