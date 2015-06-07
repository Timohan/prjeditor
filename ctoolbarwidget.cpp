#include "ctoolbarwidget.h"
#include "application.h"
#include "cbutton.h"
#include "ctexteditstylelist.h"
#include "ceditortabwidget.h"
#include "cmainwindow.h"
#include "cprojectlistwidget.h"
#include "cprojectwidget.h"
#include "ctexteditstyle.h"
#include <QGridLayout>
#include <QDebug>
#include <QComboBox>

CToolbarWidget::CToolbarWidget(CMainWindow *parent) :
    QWidget(parent),
    m_bAllowCurrentIndexChange(true),
    m_buttonNew(NULL),
    m_buttonOpen(NULL),
    m_buttonSave(NULL),
    m_buttonSaveAll(NULL),
    m_comboboxTextStyle(NULL),
    m_pMainWindow(parent)
{
    m_buttonNew = new CButton(this, STRING_TOOLBAR_NEW, "toolbar/new.png");
    m_buttonOpen = new CButton(this, STRING_TOOLBAR_OPEN, "toolbar/open.png");
    m_buttonSave = new CButton(this, STRING_TOOLBAR_SAVE, "toolbar/save.png");
    m_buttonSaveAll = new CButton(this, STRING_TOOLBAR_SAVE_ALL, "toolbar/saveall.png");
    m_comboboxTextStyle = new QComboBox(this);
    m_comboboxTabulator = new QComboBox(this);

    m_buttonSave->setEnabled(false);
    m_buttonSaveAll->setEnabled(false);

    m_buttonNew->setFlat(true);
    m_buttonOpen->setFlat(true);
    m_buttonSave->setFlat(true);
    m_buttonSaveAll->setFlat(true);

    m_comboboxTabulator->addItem(STRING_TABULATOR_4_SPACES);
    m_comboboxTabulator->addItem(STRING_TABULATOR_2_SPACES);
    m_comboboxTabulator->addItem(STRING_TABULATOR_TABULATOR);

    m_buttonNew->setGeometry(DEFAULT_MARGIN_TOOLBAR, DEFAULT_MARGIN_TOOLBAR, m_buttonNew->height(), m_buttonNew->height());
    m_buttonOpen->setGeometry(
                m_buttonNew->x() + m_buttonNew->width() + DEFAULT_MARGIN_TOOLBAR, m_buttonNew->y(),
                m_buttonNew->height(), m_buttonNew->height());
    m_buttonSave->setGeometry(
                m_buttonOpen->x() + m_buttonOpen->width() + DEFAULT_MARGIN_TOOLBAR, m_buttonOpen->y(),
                m_buttonOpen->height(), m_buttonOpen->height());
    m_buttonSaveAll->setGeometry(
                m_buttonSave->x() + m_buttonSave->width() + DEFAULT_MARGIN_TOOLBAR, m_buttonSave->y(),
                m_buttonSave->height(), m_buttonSave->height());

    m_comboboxTextStyle->setGeometry(
                m_buttonSaveAll->x() + m_buttonSaveAll->width() + DEFAULT_MARGIN_TOOLBAR, m_buttonSave->y(),
                m_comboboxTextStyle->width(), m_buttonSave->height());

    m_comboboxTabulator->setGeometry(
                m_comboboxTextStyle->x() + m_comboboxTextStyle->width() + DEFAULT_MARGIN_TOOLBAR, m_buttonSave->y(),
                150, m_buttonSave->height());


    connect(m_buttonNew, SIGNAL(clicked()), SLOT(buttonNewClicked()));
    connect(m_buttonOpen, SIGNAL(clicked()), SLOT(buttonOpenClicked()));
    connect(m_buttonSave, SIGNAL(clicked()), SLOT(buttonSaveClicked()));
    connect(m_buttonSaveAll, SIGNAL(clicked()), SLOT(buttonSaveAllClicked()));

    connect(m_comboboxTextStyle, SIGNAL(currentIndexChanged(int)), this, SLOT(textStyleCurrentIndexChanged(int)));
    connect(m_comboboxTabulator, SIGNAL(currentIndexChanged(int)), this, SLOT(textTabulatorIndexChanged(int)));

}


int CToolbarWidget::getDefaultHeight() {

    return m_buttonSave->height() + DEFAULT_MARGIN_TOOLBAR;
}


CToolbarWidget::~CToolbarWidget()
{
    if (m_buttonNew)
    {
        delete m_buttonNew;
        m_buttonNew = NULL;
    }
    if (m_buttonOpen)
    {
        delete m_buttonOpen;
        m_buttonOpen = NULL;
    }
    if (m_buttonSave)
    {
        delete m_buttonSave;
        m_buttonSave = NULL;
    }
    if (m_buttonSaveAll)
    {
        delete m_buttonSaveAll;
        m_buttonSaveAll = NULL;
    }
    if (m_comboboxTextStyle)
    {
        delete m_comboboxTextStyle;
        m_comboboxTextStyle = NULL;
    }
    if (m_comboboxTabulator) {

        delete m_comboboxTabulator;
        m_comboboxTabulator = NULL;
    }
}

int CToolbarWidget::getTabulatorIndex() {

    return m_comboboxTabulator->currentIndex();
}

void CToolbarWidget::setTabulatorIndex(int iIndex) {

    m_comboboxTabulator->setCurrentIndex(iIndex);
}

void CToolbarWidget::textTabulatorIndexChanged(int iIndex) {

    if (m_pMainWindow != NULL &&
       m_pMainWindow->getProjectListWidget() != NULL &&
       m_pMainWindow->getProjectListWidget()->getSelectedItem() != NULL &&
       m_pMainWindow->getProjectListWidget()->getSelectedItem()->getTabulatorIndex() != iIndex) {

        m_pMainWindow->getProjectListWidget()->getSelectedItem()->setTabulatorIndex(iIndex);
    }
}

void CToolbarWidget::setComboboxTextStyle(QString strStyleName)
{
    int i;

    m_bAllowCurrentIndexChange = false;

    for (i=0;i<m_comboboxTextStyle->count();i++)
    {
        if (m_comboboxTextStyle->itemText(i) == strStyleName)
        {
            m_comboboxTextStyle->setCurrentIndex(i);
            m_bAllowCurrentIndexChange = true;
            return;
        }
    }
    m_comboboxTextStyle->setCurrentIndex(0);

    m_bAllowCurrentIndexChange = true;
}

void CToolbarWidget::addTextStyleName(QString strStyleName)
{
    m_comboboxTextStyle->addItem(strStyleName);
}


void CToolbarWidget::resizeEvent(QResizeEvent *e)
{
    QWidget::resizeEvent(e);
}

void CToolbarWidget::textStyleCurrentIndexChanged(int)
{
    if (m_bAllowCurrentIndexChange == true && m_pMainWindow->getTextEditStyleList() != NULL)
    {
        CTextEditStyle *style = m_pMainWindow->getTextEditStyleList()->getTextEditStyle( m_comboboxTextStyle->currentText() );
        m_pMainWindow->getEditorTabWidget()->setTextEditStyle(style);
    }
}
void CToolbarWidget::buttonNewClicked()
{
    emit onButtonNewClicked();
}

void CToolbarWidget::buttonOpenClicked()
{
    emit openNewFile();
}

void CToolbarWidget::buttonSaveClicked()
{
    emit onSave();
}

void CToolbarWidget::buttonSaveAllClicked()
{
    emit onSaveAll();
}

void CToolbarWidget::setSaveEnabled(bool bEnabled)
{
    m_buttonSave->setEnabled(bEnabled);
    if (bEnabled)
    {
        setSaveAllEnabled(true);
    }
}

void CToolbarWidget::setSaveAllEnabled(bool bEnabled)
{
    m_buttonSaveAll->setEnabled(bEnabled);
}

