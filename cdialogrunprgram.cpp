#include <QTextEdit>
#include <QPushButton>
#include <QSettings>
#include "application.h"
#include "cdialogrunprgram.h"
#include "crunproject.h"
#include "cdialogfindcompobox.h"
#include "cdialogquerymessage.h"
#include "cprojectlistwidget.h"
#include "cprojectwidget.h"
#include "csidebarupdown.h"

CDialogRunPrgram::CDialogRunPrgram(QWidget *parent, CProjectWidget *pParentProject, CProjectListWidget *pProjectList) :
    QDialog(parent),
    m_pProject(pParentProject),
    m_pProjectListWidget(pProjectList)
{
    m_pSideBarUpDown = new CSideBarUpDown(this, this);

    m_pProjectRunProgramName = new CDialogFindCompobox(this);
    m_pProjectRunProgramName->setEditable(false);
    m_buttonSaveScript = new QPushButton(STRING_SAVE_SCRIPT, this);
    m_buttonAddRunProgramName = new QPushButton(STRING_ADD, this);


    m_texteditRunProgram = new QTextEdit(this);
    m_buttonRunProgram = new QPushButton(STRING_RUN_PROJECT, this);
    m_buttonClose = new QPushButton(STRING_PROJECT_CLOSE, this);
    m_texteditLog = new QTextEdit(this);

    m_pSideBarUpDown->setPositionY(200);

    connect(m_buttonRunProgram, SIGNAL(clicked()), this, SLOT(onRunProgram()));
    connect(m_buttonClose, SIGNAL(clicked()), this, SLOT(onClose()));

    m_pRunProgram = new CRunProject(this);
    setMinimumSize(400,300);

    connect(m_buttonAddRunProgramName, SIGNAL(clicked()), SLOT(onButtonSaveAddScriptName()));
    connect(m_buttonSaveScript, SIGNAL(clicked()), SLOT(onButtonSaveScriptClicked()));
    connect(m_pProjectRunProgramName, SIGNAL(currentIndexChanged(int)), SLOT(onProjectRunProgramNameChanged(int)));
}

void CDialogRunPrgram::loadSettings(QSettings *pSettings, int iIndex)
{
    QStringList listNames;

    m_pProjectRunProgramName->clear();


    m_listScript = pSettings->value(QString("CProjectListWidgetFilesRunProgramScript/%1").arg(iIndex) ).toStringList();
    listNames = pSettings->value(QString("CProjectListWidgetFilesRunProgramName/%1").arg(iIndex) ).toStringList();

    if (listNames.size() > 0) {

        m_pProjectRunProgramName->addItems(listNames);
    }
}

void CDialogRunPrgram::saveSettings(QSettings *pSettings, int iIndex)
{
    int i;
    QStringList listNames;

    for (i=0;i<m_pProjectRunProgramName->count();i++) {

        listNames.append(m_pProjectRunProgramName->itemText(i));
    }

    pSettings->setValue(QString("CProjectListWidgetFilesRunProgramScript/%1").arg(iIndex), m_listScript );
    pSettings->setValue(QString("CProjectListWidgetFilesRunProgramName/%1").arg(iIndex), listNames );
}

void CDialogRunPrgram::onButtonSaveAddScriptName()
{
    CDialogQueryMessage dialog(this);
    dialog.setDialog(STRING_SET_NEW_SCRIPT_NAME, STRING_SCRIPT_NAME, "");
    if (dialog.exec() == 1) {

        if (dialog.getAnswer().isEmpty() == false) {

            m_pProjectRunProgramName->addItem( dialog.getAnswer() );
            m_listScript.append( m_texteditRunProgram->toPlainText() );
            m_pProjectRunProgramName->setCurrentIndex( m_listScript.size() - 1 );
            m_pProjectListWidget->saveSettings();
        }
    }
}

void CDialogRunPrgram::onButtonSaveScriptClicked()
{
    int i;

    i = m_pProjectRunProgramName->currentIndex();

    if (i >= 0 && i < m_listScript.size()) {

        m_listScript[i] = m_texteditRunProgram->toPlainText();
        m_pProjectListWidget->saveSettings();
    }
}

void CDialogRunPrgram::onProjectRunProgramNameChanged(int i)
{
    if (i >= 0 && i < m_listScript.size()) {

        m_texteditRunProgram->setText(m_listScript.at(i));
    }
}

void CDialogRunPrgram::resizeEvent(QResizeEvent *e)
{
    QDialog::resizeEvent(e);

    refreshPosition();
}

void CDialogRunPrgram::refreshPosition() {

    m_buttonAddRunProgramName->setGeometry(DEFAULT_MARGIN_TOOLBAR, DEFAULT_MARGIN_TOOLBAR, 50, 20);
    m_pProjectRunProgramName->setGeometry(m_buttonAddRunProgramName->x()+m_buttonAddRunProgramName->width()+DEFAULT_MARGIN_TOOLBAR,
                                          DEFAULT_MARGIN_TOOLBAR,
                                          width()-150 - (m_buttonAddRunProgramName->x()+m_buttonAddRunProgramName->width()+5),
                                          20);
    m_buttonSaveScript->setGeometry(m_pProjectRunProgramName->x()+m_pProjectRunProgramName->width()+DEFAULT_MARGIN_TOOLBAR,
                                    m_pProjectRunProgramName->y(),
                                    width() - DEFAULT_MARGIN_TOOLBAR - (m_pProjectRunProgramName->x()+m_pProjectRunProgramName->width()+DEFAULT_MARGIN_TOOLBAR),
                                    m_pProjectRunProgramName->height());

    m_texteditRunProgram->setGeometry(DEFAULT_MARGIN_TOOLBAR,
                                      m_pProjectRunProgramName->y()+m_pProjectRunProgramName->height()+DEFAULT_MARGIN_TOOLBAR,
                                      width()-DEFAULT_MARGIN_TOOLBAR*2,
                                      m_pSideBarUpDown->getPositionY() - DEFAULT_MARGIN_TOOLBAR -
                                      (m_pProjectRunProgramName->y()+m_pProjectRunProgramName->height()+DEFAULT_MARGIN_TOOLBAR));

    m_buttonRunProgram->setGeometry(DEFAULT_MARGIN_TOOLBAR,
                                    m_pSideBarUpDown->getPositionY(),
                                    150,20);
    m_buttonClose->setGeometry(width()-DEFAULT_MARGIN_TOOLBAR-150,m_buttonRunProgram->y(),150,20);

    m_pSideBarUpDown->setGeometry(m_buttonRunProgram->x() + m_buttonRunProgram->width() + DEFAULT_MARGIN_TOOLBAR,
                                  m_pSideBarUpDown->getPositionY(),
                                  m_buttonClose->x() - (m_buttonRunProgram->x() + m_buttonRunProgram->width() + DEFAULT_MARGIN_TOOLBAR) -
                                  DEFAULT_MARGIN_TOOLBAR*2, m_buttonClose->height());

    m_texteditLog->setGeometry(DEFAULT_MARGIN_TOOLBAR,
                               m_buttonClose->y()+m_buttonClose->height() + DEFAULT_MARGIN_TOOLBAR,
                               width()-DEFAULT_MARGIN_TOOLBAR*2,
                               height()- DEFAULT_MARGIN_TOOLBAR*2 - (m_buttonClose->y()+m_buttonClose->height()));}

void CDialogRunPrgram::onRunProgram()
{
    QString str = m_texteditRunProgram->toPlainText();
    if (str.isEmpty() == false) {

        QStringList listCommand = str.split("\n");
        listCommand.insert(0, "cd " + m_pProject->getProjectFolderPath()  );
        m_pRunProgram->startRunProject(listCommand, m_texteditLog);
    }
}

void CDialogRunPrgram::onClose()
{
    done(0);
}
