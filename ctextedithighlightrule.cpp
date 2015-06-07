#include "ctextedithighlightrule.h"
#include <QDebug>

CTextEditHighlightRule::CTextEditHighlightRule(QObject *parent) :
    QObject(parent),
    m_iPriorityIndex(1)
{
}

int CTextEditHighlightRule::getPriorityIndex() {

    return m_iPriorityIndex;
}

void CTextEditHighlightRule::setPriorityIndex(int iPriorityIndex) {

    m_iPriorityIndex = iPriorityIndex;
}


void CTextEditHighlightRule::setRuleWord(QStringList listWord, QColor cColor)
{
    m_pType = TEXT_EDIT_STYLE_RULE_WORD;
    m_listWord = listWord;
    m_cColor = cColor;

    m_strListWordInRegExp = "\\b(";
    int i;
    for (i=0;i<m_listWord.size();i++)
    {
        if (i != 0)
        {
            m_strListWordInRegExp += "|";
        }
        m_strListWordInRegExp += m_listWord.at(i);
    }
    m_strListWordInRegExp += ")\\b";
}

QString CTextEditHighlightRule::getListWordInRegExp()
{
    return m_strListWordInRegExp;
}


void CTextEditHighlightRule::setRuleRegExp(QStringList listWord, QColor cColor)
{
    m_pType = TEXT_EDIT_STYLE_RULE_WORD_REGEXP;
    m_listWord = listWord;
    m_cColor = cColor;
    m_strListWordInRegExp = listWord.at(0);
}


void CTextEditHighlightRule::setRuleEnlightOnCursor(QStringList listWord, QColor cColor)
{
    m_pType = TEXT_EDIT_STYLE_RULE_ENLIGHT_ON_CURSOR;
    m_listWord = listWord;
    m_cColor = cColor;
}

void CTextEditHighlightRule::setRuleStartLineWithWord(QStringList listWord, QColor cColor)
{
    m_pType = TEXT_EDIT_STYLE_RULE_START_LINE_WITH_WORD;
    m_listWord = listWord;
    m_cColor = cColor;
}


void CTextEditHighlightRule::setRuleWordsBetween(QString strBegin, QString strEnd, QColor cColor, bool bSameLine, QStringList listIgnoredBegin, QStringList listIgnoredEnd)
{
    m_listWord.clear();
    m_cColor = cColor;

    if (bSameLine)
    {
        m_pType = TEXT_EDIT_STYLE_RULE_WORDS_BETWEEN_SAME_LINE;
    }
    else
    {
        m_pType = TEXT_EDIT_STYLE_RULE_WORDS_BETWEEN;
    }

    m_listIgnoredBegin = listIgnoredBegin;
    m_listIgnoredEnd = listIgnoredEnd;

    m_listWord.append(strBegin);
    m_listWord.append(strEnd);
}


QStringList CTextEditHighlightRule::getListIgnoredBegin()
{
    return m_listIgnoredBegin;
}

QStringList CTextEditHighlightRule::getListIgnoredEnd()
{
    return m_listIgnoredEnd;
}

QColor CTextEditHighlightRule::getColor()
{
    return m_cColor;
}


CTextEditHighlightRule::TextEditStyleRuleType CTextEditHighlightRule::getRuleType()
{
    return m_pType;
}

QStringList CTextEditHighlightRule::getListWord()
{
    return m_listWord;
}


