#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QDebug>
#include <QFileDialog>
#include "application.h"
#include "cdialogfindinfiles.h"
#include "cdialogfindcompobox.h"

CDialogFindInFiles::CDialogFindInFiles(QWidget *parent) :
    QWidget(parent)
{
    m_labelFiles = new QLabel(STRING_FIND_FROM, this);
    m_comboboxFiles = new CDialogFindCompobox(this);

    m_buttonOpenFolderDialog = new QPushButton("...", this);

    m_labelFileTypes = new QLabel(STRING_FIND_FILE_TYPES, this);
    m_comboboxFileTypes = new CDialogFindCompobox(this);

    connect(m_comboboxFiles, SIGNAL(enterPressed()), parent, SLOT(onSearchClicked()));
    connect(m_comboboxFileTypes, SIGNAL(enterPressed()), parent, SLOT(onSearchClicked()));

    m_comboboxFiles->setEditable(true);
    m_comboboxFileTypes->setEditable(true);

    m_labelFiles->setGeometry(0, 0, DIALOG_FIND_FIRST_COLUMN_WIDTH, m_labelFiles->height() );

    const int iButtonOpenFolderDialogWidth = 20;
    m_comboboxFiles->setGeometry(DIALOG_FIND_FIRST_COLUMN_WIDTH, 0, DIALOG_FIND_SECOND_COLUMN_WIDTH - iButtonOpenFolderDialogWidth - 5, m_comboboxFiles->height() );
    m_buttonOpenFolderDialog->setGeometry(m_comboboxFiles->x()+m_comboboxFiles->width()+5, 0, iButtonOpenFolderDialogWidth, m_comboboxFiles->height() );
    m_labelFileTypes->setGeometry(0, DIALOG_FIND_ROW_HEIGHT, DIALOG_FIND_FIRST_COLUMN_WIDTH, m_labelFileTypes->height() );
    m_comboboxFileTypes->setGeometry(DIALOG_FIND_FIRST_COLUMN_WIDTH, DIALOG_FIND_ROW_HEIGHT, DIALOG_FIND_SECOND_COLUMN_WIDTH, m_comboboxFileTypes->height() );

    connect(m_buttonOpenFolderDialog, SIGNAL(clicked()), this, SLOT(onOpenFolderDialog()));
}

void CDialogFindInFiles::onOpenFolderDialog()
{
    QString strDir = QFileDialog::getExistingDirectory(this, STRING_OPEN_FOLDER,
                                                m_comboboxFiles->currentText(),
                                                QFileDialog::ShowDirsOnly
                                                | QFileDialog::DontResolveSymlinks);

    if (strDir.isEmpty()==false)
    {
        strDir = QDir::toNativeSeparators(strDir);
        m_comboboxFiles->setCurrentText(strDir);
    }
}
void CDialogFindInFiles::setFindDirName(QString strDirName)
{
    strDirName = QDir::toNativeSeparators(strDirName);
    if (strDirName.endsWith(QDir::separator()))
    {
#ifdef WIN32
        if (strDirName.size() > 3) {
            strDirName.resize(strDirName.size()-1);
        }
#else
        if (strDirName.size() > 1) {
            strDirName.resize(strDirName.size()-1);
        }
#endif
    }
    m_comboboxFiles->setCurrentText(strDirName);
}


QString CDialogFindInFiles::getFindDirName()
{
    return QDir::toNativeSeparators(m_comboboxFiles->currentText());
}

QString CDialogFindInFiles::getFindFileTypes()
{
    return m_comboboxFileTypes->currentText();
}

bool CDialogFindInFiles::addFindValues(QString strDir, QString strFileType)
{
    int i;
    bool rv = false;

    if (strDir != m_comboboxFiles->itemText(0))
    {

        for (i=m_comboboxFiles->count()-1;i>=0;i--)
        {
            if (strDir == m_comboboxFiles->itemText(i))
            {
                m_comboboxFiles->removeItem(i);
            }
        }

        m_comboboxFiles->insertItem(0, strDir);
        rv = true;
    }


    if (strFileType != m_comboboxFileTypes->itemText(0))
    {
        for (i=m_comboboxFileTypes->count()-1;i>=0;i--)
        {
            if (strFileType == m_comboboxFileTypes->itemText(i))
            {
                m_comboboxFileTypes->removeItem(i);
            }
        }

        m_comboboxFileTypes->insertItem(0, strFileType);
        rv = true;
    }

    return rv;
}

void CDialogFindInFiles::saveValuesToSettings(QSettings *pSettings)
{
    int i;

    if (m_comboboxFiles->count() > FIND_DIALOG_SAVE_SETTINGS_MAX)
    {
        pSettings->setValue("m_comboboxFiles/Count", FIND_DIALOG_SAVE_SETTINGS_MAX);
    }
    else
    {
        pSettings->setValue("m_comboboxFiles/Count", m_comboboxFiles->count());
    }

    for (i=0;i<m_comboboxFiles->count() && i < FIND_DIALOG_SAVE_SETTINGS_MAX;i++)
    {
        pSettings->setValue(QString("m_comboboxFiles/%1").arg(i), m_comboboxFiles->itemText(i));
    }


    if (m_comboboxFileTypes->count() > FIND_DIALOG_SAVE_SETTINGS_MAX)
    {
        pSettings->setValue("m_comboboxFileTypes/Count", FIND_DIALOG_SAVE_SETTINGS_MAX);
    }
    else
    {
        pSettings->setValue("m_comboboxFileTypes/Count", m_comboboxFileTypes->count());
    }

    for (i=0;i<m_comboboxFileTypes->count() && i < FIND_DIALOG_SAVE_SETTINGS_MAX;i++)
    {
        pSettings->setValue(QString("m_comboboxFileTypes/%1").arg(i), m_comboboxFileTypes->itemText(i));
    }
}

void CDialogFindInFiles::loadValuesFromSettings(QSettings *pSettings)
{
    int i;
    int iCount = pSettings->value("m_comboboxFiles/Count", 0).toInt();

    for (i=0;i<iCount;i++)
    {
        m_comboboxFiles->addItem( pSettings->value(QString("m_comboboxFiles/%1").arg(i) ).toString() );
    }

    iCount = pSettings->value("m_comboboxFileTypes/Count", 0).toInt();

    for (i=0;i<iCount;i++)
    {
        m_comboboxFileTypes->addItem( pSettings->value(QString("m_comboboxFileTypes/%1").arg(i) ).toString() );
    }
}
