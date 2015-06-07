#ifndef CDIALOGFINDINFILES_H
#define CDIALOGFINDINFILES_H

#include <QWidget>
#include <QSettings>

class QLabel;
class QPushButton;
class CDialogFindCompobox;

class CDialogFindInFiles : public QWidget
{
    Q_OBJECT
public:
    explicit CDialogFindInFiles(QWidget *parent = 0);
    
    QString getFindDirName();
    QString getFindFileTypes();

    /**
     * @brief addFindValues
     * @param strDir
     * @param strFileType
     * @return true if the even one value is changed
     */
    bool addFindValues(QString strDir, QString strFileType);

    void loadValuesFromSettings(QSettings *pSettings);
    void saveValuesToSettings(QSettings *pSettings);

    void setFindDirName(QString strDirName);
signals:
    
public slots:
    void onOpenFolderDialog();

private:
    QLabel *m_labelFiles;
    CDialogFindCompobox *m_comboboxFiles;
    QPushButton *m_buttonOpenFolderDialog;
    QLabel *m_labelFileTypes;
    CDialogFindCompobox *m_comboboxFileTypes;

};

#endif // CDIALOGFINDINFILES_H
