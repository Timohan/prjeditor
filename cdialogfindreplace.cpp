#include <QLabel>
#include <QComboBox>
#include <QSettings>
#include "application.h"
#include "cdialogfindreplace.h"
#include "cdialogfindcompobox.h"

CDialogFindReplace::CDialogFindReplace(QWidget *parent) :
    QWidget(parent)
{
    m_labelReplace = new QLabel(STRING_FIND_REPLACE, this);
    m_comboboxReplace = new CDialogFindCompobox(this);
    m_comboboxReplace->setEditable(true);

    m_labelReplace->setGeometry(0, 0, DIALOG_FIND_FIRST_COLUMN_WIDTH, m_labelReplace->height() );
    m_comboboxReplace->setGeometry(DIALOG_FIND_FIRST_COLUMN_WIDTH, 0, DIALOG_FIND_SECOND_COLUMN_WIDTH, m_comboboxReplace->height() );

    connect(m_comboboxReplace, SIGNAL(enterPressed()), parent, SLOT(onSearchClicked()));

}

QString CDialogFindReplace::getFindToReplace()
{
    return m_comboboxReplace->currentText();
}

bool CDialogFindReplace::isChangedEnoughToSave()
{
    int i;
    QString strText = getFindToReplace();
    if (strText != m_comboboxReplace->itemText(0))
    {
        for (i=m_comboboxReplace->count()-1;i>=0;i--)
        {
            if (strText == m_comboboxReplace->itemText(i))
            {
                m_comboboxReplace->removeItem(i);
            }
        }

        m_comboboxReplace->insertItem(0, strText);

        return true;
    }

    return false;
}

void CDialogFindReplace::saveValuesToSettings(QSettings *pSettings)
{
    int i;

    if (m_comboboxReplace->count() > FIND_DIALOG_SAVE_SETTINGS_MAX)
    {
        pSettings->setValue("m_comboboxReplace/Count", FIND_DIALOG_SAVE_SETTINGS_MAX);
    }
    else
    {
        pSettings->setValue("m_comboboxReplace/Count", m_comboboxReplace->count());
    }

    for (i=0;i<m_comboboxReplace->count() && i < FIND_DIALOG_SAVE_SETTINGS_MAX;i++)
    {
        pSettings->setValue(QString("m_comboboxReplace/%1").arg(i), m_comboboxReplace->itemText(i));
    }
}

void CDialogFindReplace::loadValuesFromSettings(QSettings *pSettings)
{
    int i;
    int iCount = pSettings->value("m_comboboxReplace/Count", 0).toInt();

    for (i=0;i<iCount;i++)
    {
        m_comboboxReplace->addItem( pSettings->value(QString("m_comboboxReplace/%1").arg(i) ).toString() );
    }
}
