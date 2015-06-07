#ifndef CFINDFROMFILESTREEVIEW_H
#define CFINDFROMFILESTREEVIEW_H

#include <QTreeView>
#include <QStandardItemModel>
class CMainWindow;

class CFindFromFilesTreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit CFindFromFilesTreeView(CMainWindow *pMainWindow, QWidget *parent = 0);
    int searchFromFiles();

signals:
    
public slots:

protected:
    virtual void currentChanged(const QModelIndex &current, const QModelIndex &previous);

private:
    QStandardItemModel *m_pModel;
    CMainWindow *m_pMainWindow;
    QString m_strSearchText;

    void clear();

    void add(QString strFile, QString strLine, QString strText);
    void searchFromFile(QString strFile, const QString &strText);
    void searchFromDir(const QString &dirName, const QString &dirOriginalName);

    QList<QString>m_listSearchedDirs;
};

#endif // CFINDFROMFILESLISTWIDGET_H
