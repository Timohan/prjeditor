#include <QColor>
#include <QDebug>
#include "ctexteditstyle.h"
#include "ctextedithighlightrule.h"
#include "ctextedithighlightrules.h"

CTextEditStyle::CTextEditStyle(QObject *parent, QString strStyleName) :
    QObject(parent)
{
    m_pTextEditHighlightRules = new CTextEditHighlightRules(this);
    setStyleName(strStyleName);
}

CTextEditStyle::~CTextEditStyle()
{

    if (m_pTextEditHighlightRules) {
        delete m_pTextEditHighlightRules;
        m_pTextEditHighlightRules = NULL;
    }
}

QString CTextEditStyle::getStyleName()
{
    return m_strStyleName;
}

void CTextEditStyle::setStyleName(QString strStyleName)
{
    m_strStyleName = strStyleName;
}

int CTextEditStyle::getFileExtensionCount()
{
    return m_listFileExtension.size();
}

QString CTextEditStyle::getFileExtension(int i)
{
    return m_listFileExtension[i];
}

void CTextEditStyle::addFileExtension(QString strFileExtension)
{
    if (getFileExtensionIndex(strFileExtension) == -1)
    {
        m_listFileExtension.append(strFileExtension);
    }
}

int CTextEditStyle::getFileExtensionIndex(QString strFileExtension)
{
    int i;

    for (i=0;i<m_listFileExtension.size();i++)
    {
        if (getFileExtension(i) == strFileExtension)
        {
            return i;
        }
    }

    return -1;
}

void CTextEditStyle::removeFileExtension(QString strFileExtension)
{
    int i;

    i = getFileExtensionIndex(strFileExtension);

    if (i != -1)
    {
        removeFileExtension(i);
    }
}

void CTextEditStyle::removeFileExtension(int iIndex)
{
    m_listFileExtension.removeAt(iIndex);
}

void CTextEditStyle::addTextEditHighlightRule(CTextEditHighlightRule *pRule)
{
    m_pTextEditHighlightRules->addTextEditHighlightRule(pRule);
}

int CTextEditStyle::getTextEditHighlightRuleCount()
{
    return m_pTextEditHighlightRules->getTextEditHighlightRuleCount();
}

CTextEditHighlightRule *CTextEditStyle::getTextEditHighlightRule(int iIndex)
{
    return m_pTextEditHighlightRules->getTextEditHighlightRule(iIndex);
}

