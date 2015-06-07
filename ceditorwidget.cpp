#include "ceditorwidget.h"
#include "application.h"
#include <QTabBar>
#include "ctextedit.h"
#include "cmainwindow.h"
#include "ctexteditstylelist.h"
#include "ceditortabwidget.h"
#include "ctoolbarwidget.h"
#include "ctexteditstyle.h"
#include "crunproject.h"
#include <QDebug>
#include <QResizeEvent>
#include <QFileDialog>
#include <QLabel>

CEditorWidget::CEditorWidget(CMainWindow *pMainWindow, CEditorTabWidget *parent, QString strFileName) :
    QWidget(parent),
    m_strFileName(strFileName),
    m_pTextEdit(NULL),
    m_iId(-1),
    m_pMainWindow(pMainWindow)
{
    m_pEditorTabWidget = parent;

    m_labelCursorPosition = new QLabel(" ", this);
    m_pTextEdit = new CTextEdit(this, pMainWindow, m_labelCursorPosition);
    connect(m_pTextEdit, SIGNAL(textChanged()), this, SLOT(textChanged()) );

    m_pRunProject = new CRunProject(this);

    if (!m_strFileName.isEmpty())
    {
        setFileName(strFileName);

        QFile file(m_strFileName);

        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream in(&file);
            QString strAllText = in.readAll();
            m_pTextEdit->setPlainText(strAllText);
            file.close();
        }
    }
}

CEditorWidget::~CEditorWidget()
{
    if (m_labelCursorPosition)
    {
        delete m_labelCursorPosition;
        m_labelCursorPosition = NULL;
    }
    if (m_pTextEdit)
    {
        delete m_pTextEdit;
        m_pTextEdit = NULL;
    }
}

qint64 CEditorWidget::getId()
{
    return m_iId;
}

void CEditorWidget::setId(qint64 iId)
{
    m_iId = iId;
}

QString CEditorWidget::getFileName()
{
    return m_strFileName;
}

bool CEditorWidget::isTextSameAsSaved()
{
    if (m_pTextEdit->getLastSavedText() == m_pTextEdit->toPlainText())
    {
        return true;
    }
    else
    {
        return false;
    }
}

QString CEditorWidget::getStyleName()
{
    if (m_pTextEdit && m_pTextEdit->getStyle())
    {
        return m_pTextEdit->getStyle()->getStyleName();
    }

    return "";
}

void CEditorWidget::setTextEditStyle(CTextEditStyle *style)
{
    m_pTextEdit->setTextEditStyle(style);
}


void CEditorWidget::textChanged()
{
    if (m_pTextEdit->getLastSavedText() == m_pTextEdit->toPlainText())
    {
        m_pEditorTabWidget->textChanged(getId(), true);
    }
    else
    {
        m_pEditorTabWidget->textChanged(getId(), false);
    }
}

void CEditorWidget::setFileName(QString strFileName)
{
    m_strFileName = strFileName;

    QString strFileExtension = "";

    if (!strFileName.isEmpty())
    {
        int iLastIndexExt = strFileName.lastIndexOf(".");

        if (iLastIndexExt != -1)
        {
            strFileExtension = strFileName.right( strFileName.size() - iLastIndexExt  );

            CTextEditStyle *pStyle = m_pMainWindow->getTextEditStyleList()->getTextEditStyleByFileExtension(strFileExtension);
            if (pStyle == NULL)
            {
                pStyle = m_pMainWindow->getTextEditStyleList()->getTextEditStyle(0);
            }
            m_pMainWindow->getToolBarWidget()->setComboboxTextStyle(pStyle->getStyleName());
            m_pTextEdit->setTextEditStyle(pStyle);
        }
    }
}

CTextEdit *CEditorWidget::getTextEdit()
{
    return m_pTextEdit;
}

void CEditorWidget::resizeEvent(QResizeEvent *e)
{
    QWidget::resizeEvent(e);

    m_pTextEdit->setGeometry(0, 0, e->size().width(), e->size().height() - 20);
    m_labelCursorPosition->setGeometry(0, m_pTextEdit->y() + m_pTextEdit->height(), e->size().width(), 20);
}


QString CEditorWidget::getSelectedText() const
{
    QTextCursor cursor = m_pTextEdit->textCursor();
    return cursor.selectedText();
}

bool CEditorWidget::save(bool bToNewFile)
{
    if (m_strFileName.isEmpty())
    {
        m_strFileName = QFileDialog::getSaveFileName(this);
    }
    else if (bToNewFile)
    {
        QString strTempFileName = QFileDialog::getSaveFileName(this, QString(), m_strFileName);

        if (strTempFileName.isEmpty())
        {
            return false;
        }

        m_strFileName = strTempFileName;
    }

    if (m_strFileName.isEmpty())
    {
        return false;
    }


    QFile file(m_strFileName);

    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);
        out << m_pTextEdit->toPlainText();
        file.close();

        m_pTextEdit->setLastSavedText( m_pTextEdit->toPlainText() );

        return true;
    }

    return false;
}
