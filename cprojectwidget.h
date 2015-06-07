#ifndef CPROJECTWIDGET_H
#define CPROJECTWIDGET_H

#include <QWidget>
class CButton;
class QLabel;
class CProjectListWidget;
class CMainWindow;
class CDialogRunPrgram;
class QSettings;

class CProjectWidget : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief CProjectWidget
     * @param parent
     * @param strFolderPath
     *  folder path of project
     * @param strName
     *  name of the project
     */
    explicit CProjectWidget(CMainWindow *pMainWindow, CProjectListWidget *parent, QString strProjectFolderPath, QString strProjectName, bool bButtonClose = true, QStringList listRecentClosedFiles = QStringList(), int iTabulatorIndex = 0);
    ~CProjectWidget();

    QString getProjectFolderPath() const;
    void setProjectFolderPath(QString strProjectFolderPath);

    QString getProjectName() const;
    void setProjectName(QString strProjectName);

    bool onFolderRename(QString strFrom, QString strTo);

    void closeFile(QString strFullFilePath);
    void openFile(QString strFullFilePath);

    QStringList getRecentProjectFiles();

    void saveSettings(QSettings *pSettings, int iIndex);
    void loadSettings(QSettings *pSettings, int iIndex);

    void onRunProject();

    int getLastOpenIndex();
    void setLastOpenIndex(int iIndex);

    int getTabulatorIndex();
    void setTabulatorIndex(int iTabulatorIndex);

public slots:
    void onCloseClicked();
    void ShowContextMenu(const QPoint &pos);

protected:
    virtual void resizeEvent(QResizeEvent *event);

private:
    QString m_strProjectFolderPath;
    
    CButton *m_buttonClose;
    QLabel *m_labelProjectName;

    CProjectListWidget *m_parentProjectListWidget;
    CMainWindow *m_pMainWindow;

    QStringList m_listRecentProjectFiles;

    CDialogRunPrgram *m_dialogRunProgram;

    int m_iLastOpenIndex;

    int m_iTabulatorIndex;
};

#endif // CPROJECTWIDGET_H
