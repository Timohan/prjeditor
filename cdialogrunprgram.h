#ifndef CDIALOGRUNPRGRAM_H
#define CDIALOGRUNPRGRAM_H

#include <QDialog>
#include "isidebarupdown.h"
class QTextEdit;
class QPushButton;
class CRunProject;
class CDialogFindCompobox;
class CProjectListWidget;
class CProjectWidget;
class QSettings;
class CSideBarUpDown;

class CDialogRunPrgram : public QDialog, ISideBarUpDown
{
    Q_OBJECT
public:
    explicit CDialogRunPrgram(QWidget *parent, CProjectWidget *pParentProject, CProjectListWidget *pProjectList);

    void saveSettings(QSettings *pSettings, int iIndex);
    void loadSettings(QSettings *pSettings, int iIndex);

    virtual void refreshPosition();
signals:

public slots:
    void onRunProgram();
    void onClose();

    void onButtonSaveAddScriptName();
    void onButtonSaveScriptClicked();
    void onProjectRunProgramNameChanged(int);


protected:
    virtual void resizeEvent(QResizeEvent *e);

private:
    CSideBarUpDown *m_pSideBarUpDown;
    CProjectWidget *m_pProject;
    CProjectListWidget *m_pProjectListWidget;

    QPushButton *m_buttonAddRunProgramName;
    CDialogFindCompobox *m_pProjectRunProgramName;
    QPushButton *m_buttonSaveScript;
    QTextEdit *m_texteditRunProgram;
    QTextEdit *m_texteditLog;
    QPushButton *m_buttonRunProgram;
    QPushButton *m_buttonClose;

    CRunProject *m_pRunProgram;

    QStringList m_listScript;
};

#endif // CDIALOGRUNPRGRAM_H
