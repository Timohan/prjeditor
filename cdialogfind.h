#ifndef CDIALOGFIND_H
#define CDIALOGFIND_H

#include <QDialog>
#include <QShowEvent>

class QTabBar;
class QLabel;
class QComboBox;
class QCheckBox;
class QGroupBox;
class QRadioButton;
class CDialogFindInFiles;
class CDialogFindReplace;
class QPushButton;
class CDialogFindCompobox;

class CDialogFind : public QDialog
{
    Q_OBJECT
public:
    explicit CDialogFind(QWidget *parent = 0);

    enum EDIALOG_FIND_TYPE
    {
        DIALOG_FIND_TYPE_FIND,
        DIALOG_FIND_TYPE_FIND_IN_FILES,
        DIALOG_FIND_TYPE_FIND_REPLACE
    };

    enum EDIALOG_FIND_SEARCH_TYPE {
        EDIALOG_FIND_SEARCH_TYPE_CURRENT_DOCUMENT,
        EDIALOG_FIND_SEARCH_TYPE_ALL_OPEN_DOCUMENTS,
        EDIALOG_FIND_SEARCH_TYPE_IN_SELECTION
    };


    EDIALOG_FIND_TYPE getType();
    void setType(EDIALOG_FIND_TYPE eType);

    QString getFindText();
    QString getFindDirName();
    QString getFindFileTypes();
    QString getFindToReplace();

    Qt::CaseSensitivity getCaseSensitivity();
    bool getFindSubDirs();
    bool getFindIncludeHidden();
    /**
     * @brief getCurrentDocument
     * @return
     */
    EDIALOG_FIND_SEARCH_TYPE getSearchType();

    void setSearchValuesAsTemplate();

    bool getReplaceAll();

    bool setSelectedTextToFind(QString strSelectedText);
    void setFindDirName(QString strDirName);

signals:
    
public slots:
    void tabBarCurrentChanged(int index);
    void onCancelClicked();
    void onSearchClicked();
    void onReplaceAllClicked();

protected:
    virtual void resizeEvent(QResizeEvent *event);
    virtual void showEvent(QShowEvent * event);

private:
    EDIALOG_FIND_TYPE m_eType;

    QTabBar *m_pTabBar;
    QLabel *m_labelFind;
    CDialogFindCompobox *m_comboboxFind;

    CDialogFindInFiles *m_widgetDialogFindInFiles;
    CDialogFindReplace *m_widgetDialogReplace;

    QCheckBox *m_checkboxMatchCase;
    QCheckBox *m_checkboxSubDirs;
    QCheckBox *m_checkboxIncludeHidden;

    QGroupBox *m_groupboxSearchDocuments;
    QRadioButton *m_radiobuttonCurrentDocuments;
    QRadioButton *m_radiobuttonAllOpenDocuments;
    QRadioButton *m_radiobuttonInSelection;

    QPushButton *m_buttonCancel;
    QPushButton *m_buttonSearch;
    QPushButton *m_buttonReplaceAll;

    bool m_bReplaceAll;

};

#endif // CDIALOGFIND_H
