#include "csyntaxhighlighter.h"
#include "ctexteditstyle.h"
#include "ctextedithighlightrule.h"
#include "cmainwindow.h"
#include "ctextedit.h"
#include "cdialogfind.h"
#include <QDebug>
#include <QTextCursor>

CSyntaxHighlighter::CSyntaxHighlighter(QTextDocument *parent, CTextEdit *pTextEdit, CMainWindow *pMainWindow) :
    QSyntaxHighlighter(parent),
    m_pStyle(NULL),
    m_pMainWindow(pMainWindow),
    m_pTextEdit(pTextEdit),
    m_iHighlightingRuleEnabled(-1),
    m_eHighlightOnCursorMode(SYNTAXHIGHLIGHTER_ON_CURSOR_MODE_NONE),
    m_iBlockHighlightedOnCursorIndex(0)
{

}

void CSyntaxHighlighter::setTextEditRules(CTextEditStyle *pStyle)
{
    m_pStyle = pStyle;
}

int CSyntaxHighlighter::isValueBetween(int iValue, QList<int>&listStartIndexes, QList<int>&listEndIndexes)
{
    int i;

    for (i=0;i<listStartIndexes.size();i++)
    {
        if (listStartIndexes.at(i) <= iValue && listEndIndexes.at(i) > iValue)
        {
            return listEndIndexes.at(i);
        }
    }

    return -1;
}

CSyntaxHighlighter::ESYNTAXHIGHLIGHTER_ON_CURSOR_MODE CSyntaxHighlighter::getHighlightOnCursorMode()
{
    return m_eHighlightOnCursorMode;
}

void CSyntaxHighlighter::setHighlightOnCursorMode(ESYNTAXHIGHLIGHTER_ON_CURSOR_MODE eMode)
{
    m_eHighlightOnCursorMode = eMode;
}

int CSyntaxHighlighter::getBlockHighlightedOnCursorIndex()
{
    return this->m_iBlockHighlightedOnCursorIndex;
}

QString CSyntaxHighlighter::getHighlightOnCusorStart()
{
    return this->m_strHighlightOnCusorStart;
}

QString CSyntaxHighlighter::getHighlightOnCusorEnd()
{
    return this->m_strHighlightOnCusorEnd;
}

int CSyntaxHighlighter::getHighlightedOnCursorBlockCount()
{
    return m_listHighlightedOnCursorBlock.size();
}

QTextBlock CSyntaxHighlighter::getHighlightedOnCursorBlock(int iIndex)
{
    return m_listHighlightedOnCursorBlock.at(iIndex);
}

void CSyntaxHighlighter::clearHighlightedOnCursorBlock()
{
    m_listHighlightedOnCursorBlock.clear();
}

int CSyntaxHighlighter::highlightBlockToNext(const QString &text, int iStartPosition)
{
    int iStartCusorIndex;
    int iEndCusorIndex;


    while (1)
    {
        iStartCusorIndex = text.indexOf(m_strHighlightOnCusorStart, iStartPosition);
        iEndCusorIndex = text.indexOf(m_strHighlightOnCusorEnd, iStartPosition);

        if (iStartCusorIndex == -1 && iEndCusorIndex == -1)
        {
            return -1;
        }

        if (iEndCusorIndex == -1 ||
           (iStartCusorIndex != -1 && iStartCusorIndex < iEndCusorIndex))
        {
            iStartPosition = iStartCusorIndex + m_strHighlightOnCusorStart.size();
            if (this->format(iStartPosition).fontWeight() != QFont::Bold)
            {
                m_iBlockHighlightedOnCursorIndex++;
            }
        }
        else
        {
            iStartPosition = iEndCusorIndex + m_strHighlightOnCusorEnd.size();
            if (this->format(iStartPosition).fontWeight() != QFont::Bold)
            {
                m_iBlockHighlightedOnCursorIndex--;

                if (m_iBlockHighlightedOnCursorIndex == 0)
                {
                    QTextCharFormat keywordFormat;
                    keywordFormat.setFontWeight(QFont::Bold);
                    keywordFormat.setForeground(QBrush(m_colorHighlightOnCursor));
                    setFormat(iEndCusorIndex, m_strHighlightOnCusorEnd.size(), keywordFormat);
                    return 1;
                }
            }
        }
    }

    return 0;
}


int CSyntaxHighlighter::highlightBlockToPrevious(const QString &text, int iStartPosition)
{
    int iStartCusorIndex;
    int iEndCusorIndex;

    while (1)
    {
        iStartCusorIndex = text.lastIndexOf(m_strHighlightOnCusorStart, iStartPosition);
        iEndCusorIndex = text.lastIndexOf(m_strHighlightOnCusorEnd, iStartPosition);

        if (iStartCusorIndex == -1 && iEndCusorIndex == -1)
        {
            return -1;
        }


        if (iEndCusorIndex == -1 ||
           (iStartCusorIndex != -1 && iStartCusorIndex > iEndCusorIndex))
        {
            iStartPosition = iStartCusorIndex;
            if (this->format(iStartPosition).fontWeight() != QFont::Bold)
            {
                m_iBlockHighlightedOnCursorIndex++;

                if (m_iBlockHighlightedOnCursorIndex == 0)
                {
                    QTextCharFormat keywordFormat;
                    keywordFormat.setFontWeight(QFont::Bold);
                    keywordFormat.setForeground(QBrush(m_colorHighlightOnCursor));
                    setFormat(iStartCusorIndex, m_strHighlightOnCusorStart.size(), keywordFormat);
                    return 1;
                }
            }
        }
        else
        {
            iStartPosition = iEndCusorIndex;
            if (this->format(iStartPosition).fontWeight() != QFont::Bold)
            {
                m_iBlockHighlightedOnCursorIndex--;
            }
        }

        if (iStartPosition == 0)
        {
            break;
        }

        iStartPosition--;
    }

    return 0;
}

void CSyntaxHighlighter::setFindCursorPosition(int iFindCursorPosition)
{
    m_iFindCursorPosition = iFindCursorPosition;
}

void CSyntaxHighlighter::highlightBlockSearchAdd(const QString &text)
{
    QTextBlock currentBlock;
    QTextCharFormat keywordFormat;

    currentBlock = this->currentBlock();
    int iIndex = 0, iNewIndex, i;
    int iTextSize = m_pMainWindow->getDialogFind()->getFindText().size();

    Qt::CaseSensitivity caseSensitivity = m_pMainWindow->getDialogFind()->getCaseSensitivity();

    while (1)
    {
        iNewIndex = text.indexOf(m_pMainWindow->getDialogFind()->getFindText(), iIndex, caseSensitivity);
        if (iNewIndex == -1)
        {
            return;
        }

        for (i=0;i<iTextSize;i++)
        {
            keywordFormat = format(iNewIndex+i);
            keywordFormat.setBackground(QBrush(QColor(114, 249, 187)));
            setFormat(iNewIndex+i, 1, keywordFormat);
        }

        iIndex = iNewIndex + iTextSize;
    }
}

void CSyntaxHighlighter::highlightBlock(const QString &text)
{
    int i;
    CTextEditHighlightRule *pRule;
    int iStartIndex;
    int iEndIndex;
    int iIndex = 0;
    int i2;
    int index = 0;
    int iCursorPosition;
    bool bDone = false;
    QTextBlock currentBlock;
    QTextCursor cursor;
    QTextCharFormat keywordFormat;

    setCurrentBlockState(-1);

    if (m_pStyle == NULL)
    {
        if (m_eHighlightOnCursorMode == SYNTAXHIGHLIGHTER_ON_CURSOR_MODE_SEARCH_ADD)
        {
            highlightBlockSearchAdd(text);
        }
        return;
    }

    keywordFormat.setFontWeight(QFont::Bold);


    currentBlock = this->currentBlock();
    cursor = m_pTextEdit->textCursor();
    SHighlightIndexes pHighlightIndexes;

    if (previousBlockState() != -1)
    {
        pRule = m_pStyle->getTextEditHighlightRule( previousBlockState() );
        keywordFormat.setForeground(QBrush(pRule->getColor()));

        iEndIndex = text.indexOf(pRule->getListWord().at(1));

        if (iEndIndex == -1)
        {
            setCurrentBlockState(previousBlockState());
            setFormat(0, text.size(), keywordFormat);
            if (m_eHighlightOnCursorMode == SYNTAXHIGHLIGHTER_ON_CURSOR_MODE_SEARCH_ADD)
            {
                highlightBlockSearchAdd(text);
            }
            return;
        }
        else
        {
            index = iEndIndex + pRule->getListWord().at(1).size();
            setFormat(0, index, keywordFormat);
            pHighlightIndexes.m_listStartIndexes.append(0);
            pHighlightIndexes.m_listEndIndexes.append(index);
        }

    }

    int iNewStartIndex;
    int iRuleIndex = -1;
    int iNewLength = 0;
    int iLength = 0;
    int iPlus = 0;
    int iNewPlus = 0;
    while (1) {

        iPlus = 0;
        iNewLength = 1;
        iStartIndex = -1;
        for (i=0;i<m_pStyle->getTextEditHighlightRuleCount();i++) {

            iNewStartIndex = -1;
            pRule = m_pStyle->getTextEditHighlightRule(i);
            switch (pRule->getRuleType())
            {
                case CTextEditHighlightRule::TEXT_EDIT_STYLE_RULE_WORDS_BETWEEN:
                    iNewStartIndex = text.indexOf(pRule->getListWord().at(0), index);
                    keywordFormat.setForeground(QBrush(pRule->getColor()));
                    if (iNewStartIndex != -1)
                    {
                        iEndIndex = text.indexOf(pRule->getListWord().at(1), iNewStartIndex+pRule->getListWord().at(0).size());
                        if (iEndIndex == -1)
                        {
                            setCurrentBlockState(iIndex);
                            iNewLength = text.size()-iNewStartIndex;
                        }
                        else
                        {
                            iNewLength = iEndIndex-iNewStartIndex+pRule->getListWord().at(1).size();
                        }
                        iNewPlus = 0;
                    }

                case CTextEditHighlightRule::TEXT_EDIT_STYLE_RULE_ENLIGHT_ON_CURSOR:
                    break;

                case CTextEditHighlightRule::TEXT_EDIT_STYLE_RULE_WORDS_BETWEEN_SAME_LINE:
                    if (pRule->getListIgnoredBegin().size() == 0)
                    {
                        iNewStartIndex = text.indexOf(pRule->getListWord().at(0), index);
                        if (iNewStartIndex != -1) {
                            iNewLength = text.indexOf(pRule->getListWord().at(1), iStartIndex+pRule->getListWord().at(0).size()) - iNewStartIndex;
                            if (iNewLength <= 0) {
                                iNewLength = -1;
                            }
                            iNewPlus = 1;
                        }
                    }
                    else {
                        iNewStartIndex = findNextStringLocation(text, index, pRule->getListWord().at(0), pRule->getListIgnoredBegin());
                        if (iNewStartIndex != -1) {

                            iNewLength = findNextStringLocation(text,
                                                                iNewStartIndex+pRule->getListWord().at(0).size(),
                                                                pRule->getListWord().at(1),
                                                                pRule->getListIgnoredEnd())-iNewStartIndex;
                            if (iNewLength <= -1) {
                                iNewLength = -1;
                            }
                            iNewPlus = 1;
                        }
                    }
                    break;

                default:
                    QRegExp expression(pRule->getListWordInRegExp());
                    iNewStartIndex = expression.indexIn(text, index);

                    iNewLength = expression.matchedLength();

                    if (iNewLength == 0) {
                        iNewStartIndex = -1;
                    }
                    iNewPlus = 0;
                    break;

            }

            if (iNewStartIndex != -1) {

                if (iStartIndex == -1 || iNewStartIndex < iStartIndex) {
                    iStartIndex = iNewStartIndex;
                    iRuleIndex = i;
                    iLength = iNewLength;
                    iPlus = iNewPlus;
                }
            }
        }

        if (iStartIndex == -1) {
            break;
        }

        if (iLength == -1) {
            iLength = text.length() - iStartIndex;
        }

        index = highlightBlockLineByRule(iStartIndex, iLength, iRuleIndex, &pHighlightIndexes) + iPlus;

        if (index >= text.length()) {
            break;
        }
    }


    for (i=0;i<m_pStyle->getTextEditHighlightRuleCount();i++) {

        if (CTextEditHighlightRule::TEXT_EDIT_STYLE_RULE_ENLIGHT_ON_CURSOR == pRule->getRuleType()) {

            if (m_eHighlightOnCursorMode == SYNTAXHIGHLIGHTER_ON_CURSOR_MODE_NORMAL &&
                cursor.selectionStart() >= currentBlock.position() &&
                cursor.selectionStart() < currentBlock.position() + currentBlock.length())
            {
                iCursorPosition = cursor.selectionStart() - currentBlock.position();

                if (isValueBetween(iCursorPosition, pHighlightIndexes.m_listStartIndexes, pHighlightIndexes.m_listEndIndexes) == -1)
                {
                    keywordFormat.setForeground(QBrush(pRule->getColor()));
                    QString strTemp = text.right(text.size() - iCursorPosition );
                    for (i2=0;i2<pRule->getListWord().size() && bDone == false;i2+=2)
                    {
                        if (strTemp.startsWith(pRule->getListWord().at(i2)))
                        {
                            setFormat(iCursorPosition, pRule->getListWord().at(i2).size(), keywordFormat);
                            pHighlightIndexes.m_listStartIndexes.append(iCursorPosition);
                            pHighlightIndexes.m_listEndIndexes.append(iCursorPosition + pRule->getListWord().at(i2).size());

                            m_strHighlightOnCusorStart = pRule->getListWord().at(i2);
                            m_strHighlightOnCusorEnd = pRule->getListWord().at(i2+1);
                            m_colorHighlightOnCursor = pRule->getColor();


                            m_iBlockHighlightedOnCursorIndex = 1;
                            m_listHighlightedOnCursorBlock.append(currentBlock);

                            highlightBlockToNext(text, iCursorPosition + pRule->getListWord().at(i2).size() );

                            bDone = true;
                            break;
                        }
                    }

                    for (i2=1;i2<pRule->getListWord().size() && bDone == false;i2+=2)
                    if (iCursorPosition-pRule->getListWord().at(i2).size() >= 0)
                    {
                        strTemp = text.right(text.size() - iCursorPosition + pRule->getListWord().at(i2).size() );
                        if (strTemp.startsWith(pRule->getListWord().at(i2)))
                        {
                            setFormat(iCursorPosition-pRule->getListWord().at(i2).size(), pRule->getListWord().at(i2).size(), keywordFormat);
                            pHighlightIndexes.m_listStartIndexes.append(iCursorPosition-pRule->getListWord().at(i2).size());
                            pHighlightIndexes.m_listEndIndexes.append(iCursorPosition);

                            m_strHighlightOnCusorStart = pRule->getListWord().at(i2-1);
                            m_strHighlightOnCusorEnd = pRule->getListWord().at(i2);
                            m_colorHighlightOnCursor = pRule->getColor();

                            m_iBlockHighlightedOnCursorIndex = -1;
                            m_listHighlightedOnCursorBlock.append(currentBlock);

                            if (iCursorPosition - pRule->getListWord().at(i2).size() > 0)
                            {
                                highlightBlockToPrevious(text, iCursorPosition - pRule->getListWord().at(i2).size() - 1 );
                            }
                            bDone = true;
                            break;
                        }
                    }
                }
            }
        }
    }

    if (m_eHighlightOnCursorMode == SYNTAXHIGHLIGHTER_ON_CURSOR_MODE_ADD &&
        m_iBlockHighlightedOnCursorIndex != 0)
    {
        if (m_iBlockHighlightedOnCursorIndex > 0)
        {
            highlightBlockToNext(text, 0);
        }
        else
        {
            highlightBlockToPrevious(text, -1);
        }

        if (m_iBlockHighlightedOnCursorIndex == 0)
        {
            m_listHighlightedOnCursorBlock.append(currentBlock);
        }
    }
    if (m_eHighlightOnCursorMode == SYNTAXHIGHLIGHTER_ON_CURSOR_MODE_SEARCH_ADD)
    {
        highlightBlockSearchAdd(text);
    }
}

int CSyntaxHighlighter::highlightBlockLineByRule(int iStartIndex, int iLength, int iRuleIndex, SHighlightIndexes *pHighlightIndexes) {

    CTextEditHighlightRule *pRule;
    QTextCharFormat keywordFormat;
    keywordFormat.setFontWeight(QFont::Bold);
    pRule = m_pStyle->getTextEditHighlightRule(iRuleIndex);
    switch (pRule->getRuleType())
    {
        case CTextEditHighlightRule::TEXT_EDIT_STYLE_RULE_WORDS_BETWEEN:
            keywordFormat.setForeground(QBrush(pRule->getColor()));
            setFormat(iStartIndex, iLength, keywordFormat);
            pHighlightIndexes->m_listStartIndexes.append(iStartIndex);
            pHighlightIndexes->m_listEndIndexes.append(iStartIndex + iLength);

            break;

        case CTextEditHighlightRule::TEXT_EDIT_STYLE_RULE_ENLIGHT_ON_CURSOR:
            break;

        case CTextEditHighlightRule::TEXT_EDIT_STYLE_RULE_WORDS_BETWEEN_SAME_LINE:
            keywordFormat.setForeground(QBrush(pRule->getColor()));
            setFormat(iStartIndex, iLength, keywordFormat);
            pHighlightIndexes->m_listStartIndexes.append(iStartIndex);
            pHighlightIndexes->m_listEndIndexes.append(iStartIndex + iLength);
            break;

        default:
            keywordFormat.setForeground(QBrush(pRule->getColor()));
            setFormat(iStartIndex, iLength, keywordFormat);
            pHighlightIndexes->m_listStartIndexes.append(iStartIndex);
            pHighlightIndexes->m_listEndIndexes.append(iStartIndex + iLength);
            break;
    }

    return iStartIndex + iLength;
}

int CSyntaxHighlighter::findNextStringLocation(const QString &strString, int iStartPosition, QString strStringToFind, QStringList listToIgnore)
{
    int iStartIndexNew = iStartPosition;
    int iStartIndexNew2;
    int iStartIndexNewToIgnore;
    int i;
    bool bDone;


    while (1)
    {
        iStartIndexNew2 = strString.indexOf(strStringToFind, iStartIndexNew);

        if (iStartIndexNew2 == -1)
        {
            return -1;
        }

        bDone = true;
        for (i=0;i<listToIgnore.size();i++)
        {
            iStartIndexNewToIgnore = strString.indexOf(listToIgnore.at(i), iStartIndexNew);

            if (iStartIndexNewToIgnore != -1 &&
                iStartIndexNewToIgnore + listToIgnore.at(i).size() == iStartIndexNew2 + strStringToFind.size())
            {
                iStartIndexNew = iStartIndexNew2 + strStringToFind.size();
                bDone = false;
                break;
            }
        }

        if (bDone)
        {
            return iStartIndexNew2;
        }
    }

    return -1;
}
