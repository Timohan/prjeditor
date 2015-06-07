#include "ctextedithighlightrule.h"
#include "ctextedithighlightrules.h"

CTextEditHighlightRules::CTextEditHighlightRules(QObject *parent) :
    QObject(parent)
{


}

CTextEditHighlightRules::~CTextEditHighlightRules()
{

    int i;
    for (i=0;i<m_pTextEditHighlightRule.size();i++)
    {
        delete m_pTextEditHighlightRule[i];
        m_pTextEditHighlightRule[i] = NULL;
    }

    m_pTextEditHighlightRule.clear();
}

void CTextEditHighlightRules::addTextEditHighlightRule(CTextEditHighlightRule *pRule)
{
    m_pTextEditHighlightRule.append(pRule);
}

int CTextEditHighlightRules::getTextEditHighlightRuleCount()
{
    return m_pTextEditHighlightRule.size();
}

CTextEditHighlightRule *CTextEditHighlightRules::getTextEditHighlightRule(int iIndex)
{
    return m_pTextEditHighlightRule.at(iIndex);
}
