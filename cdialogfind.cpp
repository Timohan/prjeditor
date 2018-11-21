#include "application.h"
#include <QTabBar>
#include <QLabel>
#include <QCheckBox>
#include <QComboBox>
#include <QGroupBox>
#include <QPushButton>
#include <QRadioButton>
#include <QSettings>
#include "cdialogfind.h"
#include "cdialogfindinfiles.h"
#include "cdialogfindreplace.h"
#include "cdialogfindcompobox.h"

CDialogFind::CDialogFind(QWidget *parent) :
    QDialog(parent),
    m_eType(DIALOG_FIND_TYPE_FIND)
{
    this->setWindowTitle(STRING_FIND);

    m_pTabBar = new QTabBar(this);
    m_pTabBar->addTab(STRING_MENU_FIND);
    m_pTabBar->addTab(STRING_MENU_FIND_IN_FILES);
    m_pTabBar->addTab(STRING_MENU_REPLACE);

    m_labelFind = new QLabel(STRING_MENU_FIND, this);

    m_checkboxMatchCase = new QCheckBox(STRING_FIND_MATCH_CASE, this);
    m_checkboxMatchCase->setChecked(true);
    m_checkboxSubDirs = new QCheckBox(STRING_FIND_INCLUDE_SUB_FOLDERS, this);
    m_checkboxIncludeHidden = new QCheckBox(STRING_FIND_INCLUDE_HIDDEN, this);

    m_groupboxSearchDocuments = new QGroupBox(STRING_FIND_SEARCH, this);
    m_radiobuttonCurrentDocuments = new QRadioButton(STRING_FIND_CURRENT_DOCUMENT, m_groupboxSearchDocuments);
    m_radiobuttonAllOpenDocuments = new QRadioButton(STRING_FIND_ALL_OPEN_DOCUMENTS, m_groupboxSearchDocuments);
    m_radiobuttonInSelection = new QRadioButton(STRING_FIND_IN_SELECTED_TEXT, m_groupboxSearchDocuments);
    m_radiobuttonCurrentDocuments->setChecked(true);

    m_buttonCancel = new QPushButton(STRING_CANCEL, this);
    m_buttonSearch = new QPushButton(STRING_FIND, this);
    m_buttonReplaceAll = new QPushButton(STRING_REPLACE_ALL, this);



    m_comboboxFind = new CDialogFindCompobox(this);
    m_comboboxFind->setEditable(true);

    m_widgetDialogFindInFiles = new CDialogFindInFiles(this);
    m_widgetDialogReplace = new CDialogFindReplace(this);

    m_widgetDialogFindInFiles->setVisible(false);
    m_widgetDialogReplace->setVisible(false);
    m_checkboxSubDirs->setVisible(false);
    m_checkboxIncludeHidden->setVisible(false);

    connect(m_pTabBar, SIGNAL(currentChanged(int)), this, SLOT(tabBarCurrentChanged(int)));

    connect(m_buttonCancel, SIGNAL(clicked()), this, SLOT(onCancelClicked()));
    connect(m_buttonSearch, SIGNAL(clicked()), this, SLOT(onSearchClicked()));
    connect(m_buttonReplaceAll, SIGNAL(clicked()), this, SLOT(onReplaceAllClicked()));

    connect(m_comboboxFind, SIGNAL(enterPressed()), this, SLOT(onSearchClicked()));
  //  connect(m_widgetDialogFindInFiles, SIGNAL(enterPressed()), this, SLOT(onSearchClicked()));
  //  connect(m_widgetDialogReplace, SIGNAL(enterPressed()), this, SLOT(onSearchClicked()));

    this->resizeEvent(nullptr);

    int h = m_groupboxSearchDocuments->y()+
            m_groupboxSearchDocuments->height()+
            m_buttonSearch->height()+DEFAULT_MARGIN*2;
    this->setMinimumWidth(400);
    this->setMaximumWidth(400);
    this->setMinimumHeight(h);
    this->setMaximumHeight(h);
    this->resize(400, h);
    this->setFixedSize(this->width(),this->height());


    QSettings settings(STRING_ORGANIZATION, STRING_APPLICATION_NAME);
    int iCount = settings.value("m_comboboxFind/Count", 0).toInt();
    int i;

    for (i=0;i<iCount;i++)
    {
        m_comboboxFind->addItem( settings.value(QString("m_comboboxFind/%1").arg(i) ).toString() );
    }

    m_widgetDialogFindInFiles->loadValuesFromSettings(&settings);
    m_widgetDialogReplace->loadValuesFromSettings(&settings);
}

void CDialogFind::resizeEvent(QResizeEvent *event)
{
    int y;

    if (event)
    {
        QDialog::resizeEvent(event);
    }

    m_labelFind->setGeometry(DEFAULT_MARGIN, DIALOG_FIND_ROW_HEIGHT,
                             DIALOG_FIND_FIRST_COLUMN_WIDTH, m_labelFind->height() );

    m_comboboxFind->setGeometry(DIALOG_FIND_FIRST_COLUMN_WIDTH+DEFAULT_MARGIN, m_labelFind->y(),
                             DIALOG_FIND_SECOND_COLUMN_WIDTH, m_comboboxFind->height() );

    switch (m_eType)
    {
        case DIALOG_FIND_TYPE_FIND:
        default:
            y = m_labelFind->y() + DIALOG_FIND_ROW_HEIGHT;
            break;
        case DIALOG_FIND_TYPE_FIND_REPLACE:
            m_widgetDialogReplace->setGeometry(DEFAULT_MARGIN, m_labelFind->y() + DIALOG_FIND_ROW_HEIGHT,
                                               DIALOG_FIND_FIRST_COLUMN_WIDTH + DIALOG_FIND_SECOND_COLUMN_WIDTH,
                                               DIALOG_FIND_ROW_HEIGHT);
            y = m_widgetDialogReplace->y() + m_widgetDialogReplace->height();
            break;
        case DIALOG_FIND_TYPE_FIND_IN_FILES:
            m_widgetDialogFindInFiles->setGeometry(DEFAULT_MARGIN, m_labelFind->y() + DIALOG_FIND_ROW_HEIGHT,
                                               DIALOG_FIND_FIRST_COLUMN_WIDTH + DIALOG_FIND_SECOND_COLUMN_WIDTH,
                                               DIALOG_FIND_ROW_HEIGHT*2);
            y = m_widgetDialogFindInFiles->y() + m_widgetDialogFindInFiles->height();
            break;
    }

    m_checkboxMatchCase->setGeometry(DEFAULT_MARGIN, y, DIALOG_FIND_FIRST_COLUMN_WIDTH, DIALOG_FIND_ROW_HEIGHT);

    switch (m_eType)
    {
        case DIALOG_FIND_TYPE_FIND_IN_FILES:
            m_checkboxSubDirs->setGeometry(m_checkboxMatchCase->x(), y+DIALOG_FIND_ROW_HEIGHT, DIALOG_FIND_FIRST_COLUMN_WIDTH, DIALOG_FIND_ROW_HEIGHT);
            m_checkboxIncludeHidden->setGeometry(m_checkboxMatchCase->x(), y+DIALOG_FIND_ROW_HEIGHT*2, DIALOG_FIND_FIRST_COLUMN_WIDTH, DIALOG_FIND_ROW_HEIGHT);
            break;
        default:
            break;
    }

    m_groupboxSearchDocuments->setGeometry(DIALOG_FIND_FIRST_COLUMN_WIDTH+DEFAULT_MARGIN, y, DIALOG_FIND_SECOND_COLUMN_WIDTH,  DIALOG_FIND_ROW_HEIGHT*4);
    m_radiobuttonCurrentDocuments->setGeometry(10,15, DIALOG_FIND_SECOND_COLUMN_WIDTH-20, m_radiobuttonCurrentDocuments->height());
    m_radiobuttonAllOpenDocuments->setGeometry(m_radiobuttonCurrentDocuments->x(),
                                               m_radiobuttonCurrentDocuments->y()+m_radiobuttonCurrentDocuments->height(),
                                               DIALOG_FIND_SECOND_COLUMN_WIDTH-20, m_radiobuttonAllOpenDocuments->height());
    m_radiobuttonInSelection->setGeometry(m_radiobuttonCurrentDocuments->x(),
                                               m_radiobuttonAllOpenDocuments->y()+m_radiobuttonAllOpenDocuments->height(),
                                               DIALOG_FIND_SECOND_COLUMN_WIDTH-20, m_radiobuttonInSelection->height());


    m_buttonSearch->setGeometry(width() - DEFAULT_MARGIN - m_buttonSearch->width(),
                                height() - DEFAULT_MARGIN - m_buttonSearch->height(),
                                m_buttonSearch->width(),
                                m_buttonSearch->height());
    m_buttonReplaceAll->setGeometry(m_buttonSearch->x() - DEFAULT_MARGIN - m_buttonCancel->width(),
                                m_buttonSearch->y(),
                                m_buttonCancel->width(),
                                m_buttonCancel->height());

    m_buttonCancel->setGeometry(DEFAULT_MARGIN,
                                m_buttonSearch->y(),
                                m_buttonCancel->width(),
                                m_buttonCancel->height());



}

void CDialogFind::showEvent(QShowEvent *e)
{
    QDialog::showEvent(e);

    if (this->isVisible())
    {
        m_comboboxFind->setFocus();
    }
}

void CDialogFind::tabBarCurrentChanged(int index)
{
    if (index == 0)
    {
        m_eType = DIALOG_FIND_TYPE_FIND;
        m_widgetDialogFindInFiles->setVisible(false);
        m_widgetDialogReplace->setVisible(false);
        m_checkboxSubDirs->setVisible(false);
        m_checkboxIncludeHidden->setVisible(false);
        m_groupboxSearchDocuments->setEnabled(true);
        m_radiobuttonInSelection->setEnabled(false);
        m_buttonSearch->setText(STRING_FIND);
        m_buttonReplaceAll->setEnabled(false);

        if (m_radiobuttonInSelection->isChecked())
        {
            m_radiobuttonCurrentDocuments->setChecked(true);
        }
    }
    else if (index == 1)
    {
        m_eType = DIALOG_FIND_TYPE_FIND_IN_FILES;
        m_widgetDialogFindInFiles->setVisible(true);
        m_widgetDialogReplace->setVisible(false);
        m_checkboxSubDirs->setVisible(true);
        m_checkboxIncludeHidden->setVisible(true);
        m_groupboxSearchDocuments->setEnabled(false);
        m_buttonSearch->setText(STRING_FIND);
        m_buttonReplaceAll->setEnabled(false);
    }
    else
    {
        m_eType = DIALOG_FIND_TYPE_FIND_REPLACE;
        m_widgetDialogFindInFiles->setVisible(false);
        m_widgetDialogReplace->setVisible(true);
        m_checkboxSubDirs->setVisible(false);
        m_checkboxIncludeHidden->setVisible(false);
        m_groupboxSearchDocuments->setEnabled(true);
        m_radiobuttonInSelection->setEnabled(true);
        m_buttonSearch->setText(STRING_MENU_REPLACE);
        m_buttonReplaceAll->setEnabled(true);
    }

    this->resizeEvent(nullptr);
    int h = m_groupboxSearchDocuments->y()+
            m_groupboxSearchDocuments->height()+
            m_buttonSearch->height()+DEFAULT_MARGIN*2;
    this->setMinimumWidth(400);
    this->setMaximumWidth(400);
    this->setMinimumHeight(h);
    this->setMaximumHeight(h);
    this->resize(400, h);
    this->setFixedSize(this->width(),this->height());

}

CDialogFind::EDIALOG_FIND_SEARCH_TYPE CDialogFind::getSearchType()
{
    if (m_radiobuttonCurrentDocuments->isChecked())
    {
        return EDIALOG_FIND_SEARCH_TYPE_CURRENT_DOCUMENT;
    }
    else if (m_radiobuttonAllOpenDocuments->isChecked())
    {
        return EDIALOG_FIND_SEARCH_TYPE_ALL_OPEN_DOCUMENTS;
    }

    return EDIALOG_FIND_SEARCH_TYPE_IN_SELECTION;
}

QString CDialogFind::getFindText()
{
    return m_comboboxFind->currentText();
}

QString CDialogFind::getFindDirName()
{
    return m_widgetDialogFindInFiles->getFindDirName();
}

void CDialogFind::setFindDirName(QString strDirName)
{
    m_widgetDialogFindInFiles->setFindDirName(strDirName);
}

QString CDialogFind::getFindFileTypes()
{
    return m_widgetDialogFindInFiles->getFindFileTypes();
}

QString CDialogFind::getFindToReplace()
{
    return m_widgetDialogReplace->getFindToReplace();
}


Qt::CaseSensitivity CDialogFind::getCaseSensitivity()
{
    if (m_checkboxMatchCase->checkState() == Qt::Checked)
    {
        return Qt::CaseSensitive;
    }
    else
    {
        return Qt::CaseInsensitive;
    }
}

bool CDialogFind::getFindSubDirs()
{
    if (m_checkboxSubDirs->checkState() == Qt::Unchecked)
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool CDialogFind::getFindIncludeHidden()
{
    if (m_checkboxIncludeHidden->checkState() == Qt::Unchecked)
    {
        return false;
    }
    else
    {
        return true;
    }
}


CDialogFind::EDIALOG_FIND_TYPE CDialogFind::getType()
{
    return m_eType;
}

void CDialogFind::setType(EDIALOG_FIND_TYPE eType)
{
    switch (eType)
    {
        case DIALOG_FIND_TYPE_FIND:
            m_pTabBar->setCurrentIndex(0);
            break;
        case DIALOG_FIND_TYPE_FIND_IN_FILES:
            m_pTabBar->setCurrentIndex(1);
            break;
        case DIALOG_FIND_TYPE_FIND_REPLACE:
            m_pTabBar->setCurrentIndex(2);
            break;
    }

}


void CDialogFind::onCancelClicked()
{
    this->done(0);
}

void CDialogFind::setSearchValuesAsTemplate()
{
    int i;
    bool bChanged0 = false;
    bool bChanged1 = false;
    if (getFindText() != m_comboboxFind->itemText(0))
    {
        QString strText = getFindText();

        for (i=m_comboboxFind->count()-1;i>=0;i--)
        {
            if (strText == m_comboboxFind->itemText(i))
            {
                m_comboboxFind->removeItem(i);
            }
        }

        m_comboboxFind->insertItem(0, strText);
        bChanged0 = true;
    }

    if (getType() == DIALOG_FIND_TYPE_FIND_IN_FILES)
    {
        bChanged1 = m_widgetDialogFindInFiles->addFindValues(getFindDirName(), getFindFileTypes());
    }
    else if (getType() == DIALOG_FIND_TYPE_FIND_REPLACE)
    {
        bChanged1 = m_widgetDialogReplace->isChangedEnoughToSave();
    }

    if (bChanged0 || bChanged1)
    {
        QSettings settings(STRING_ORGANIZATION, STRING_APPLICATION_NAME);

        if (m_comboboxFind->count() > FIND_DIALOG_SAVE_SETTINGS_MAX)
        {
            settings.setValue("m_comboboxFind/Count", FIND_DIALOG_SAVE_SETTINGS_MAX);
        }
        else
        {
            settings.setValue("m_comboboxFind/Count", m_comboboxFind->count());
        }

        for (i=0;i<m_comboboxFind->count() && i < FIND_DIALOG_SAVE_SETTINGS_MAX;i++)
        {
            settings.setValue(QString("m_comboboxFind/%1").arg(i), m_comboboxFind->itemText(i));
        }

        if (getType() == DIALOG_FIND_TYPE_FIND_IN_FILES)
        {
            m_widgetDialogFindInFiles->saveValuesToSettings(&settings);
        }
        else if (getType() == DIALOG_FIND_TYPE_FIND_REPLACE) {
            m_widgetDialogReplace->saveValuesToSettings(&settings);
        }
    }

    m_comboboxFind->setCurrentIndex(0);
}

void CDialogFind::onSearchClicked()
{
    m_bReplaceAll = false;
    this->done(1);
}

void CDialogFind::onReplaceAllClicked()
{
    m_bReplaceAll = true;
    this->done(1);
}

bool CDialogFind::getReplaceAll()
{
    return m_bReplaceAll;
}

bool CDialogFind::setSelectedTextToFind(QString strSelectedText)
{
    if (strSelectedText.isEmpty() == true ||
        (strSelectedText.indexOf(" ") && strSelectedText.size() > 25) ||
        strSelectedText.indexOf("\r") != -1 ||
        strSelectedText.indexOf("\n") != -1) {

        return false;
    }

    m_comboboxFind->setCurrentText(strSelectedText);

    return true;
}
