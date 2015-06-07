#ifndef CSYNTAXHIGHLIGHTER_H
#define CSYNTAXHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextDocument>

class CTextEdit;
class CMainWindow;
class CTextEditStyle;
#include "ctextedithighlightrule.h"


class CSyntaxHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    struct SHighlightIndexes {
        QList<int> m_listStartIndexes;
        QList<int> m_listEndIndexes;
        int rvLastEndIndex;
    };

    explicit CSyntaxHighlighter(QTextDocument *parent, CTextEdit *pTextEdit, CMainWindow *pMainWindow);
    void setTextEditRules(CTextEditStyle *pStyle);

    enum ESYNTAXHIGHLIGHTER_ON_CURSOR_MODE {
        SYNTAXHIGHLIGHTER_ON_CURSOR_MODE_NONE,
        SYNTAXHIGHLIGHTER_ON_CURSOR_MODE_NORMAL,
        SYNTAXHIGHLIGHTER_ON_CURSOR_MODE_REMOVE,
        SYNTAXHIGHLIGHTER_ON_CURSOR_MODE_ADD,
        SYNTAXHIGHLIGHTER_ON_CURSOR_MODE_SEARCH_ADD, // used when user does the search
    };

    ESYNTAXHIGHLIGHTER_ON_CURSOR_MODE getHighlightOnCursorMode();
    void setHighlightOnCursorMode(ESYNTAXHIGHLIGHTER_ON_CURSOR_MODE eMode);
    int getBlockHighlightedOnCursorIndex();
    QString getHighlightOnCusorStart();
    QString getHighlightOnCusorEnd();

    int getHighlightedOnCursorBlockCount();
    QTextBlock getHighlightedOnCursorBlock(int iIndex);
    void clearHighlightedOnCursorBlock();
    void setFindCursorPosition(int iFindCursorPosition);

signals:
    
public slots:

protected:
    void highlightBlock(const QString &text);

private:

    CTextEditStyle *m_pStyle;
    CMainWindow *m_pMainWindow;
    CTextEdit *m_pTextEdit;

    /**
     * @brief isValueBetween
     * is iValue between startIndexes and endIndexes
     * @param iValue
     * @param listStartIndexes
     * @param listEndIndexes
     * @return -1 if not
     * returns endindex if yes >= 0
     */
    int isValueBetween(int iValue, QList<int>&listStartIndexes, QList<int>&listEndIndexes);
    int m_iHighlightingRuleEnabled;
    int m_iFindCursorPosition;

    ESYNTAXHIGHLIGHTER_ON_CURSOR_MODE m_eHighlightOnCursorMode;

    QList<QTextBlock>m_listHighlightedOnCursorBlock;
    int m_iBlockHighlightedOnCursorIndex;
    QString m_strHighlightOnCusorStart;
    QString m_strHighlightOnCusorEnd;
    QColor m_colorHighlightOnCursor;

    int highlightBlockToNext(const QString &text, int iStartPosition);
    int highlightBlockToPrevious(const QString &text, int iStartPosition);

    /**
     * @brief highlightBlockSearchAdd
     * highlight on search adding (making background for searched text after all other
     * highlighting is done
     * @param text
     */
    void highlightBlockSearchAdd(const QString &text);

    int findNextStringLocation(const QString &strString, int iStartPosition, QString strStringToFind, QStringList listToIgnore);

    /**
     * @brief highlightBlockLineByRule colors the highly by rule
     * @param text, text to color
     * @param iStartIndex, start index of the highlight in text
     * @param iRuleIndex, which rule index in rule list
     * @param pHighlightIndexes, list of start/end indexes that are already in the use (highlighted)
     * @return end index
     */
    int highlightBlockLineByRule(int iStartIndex,  int iLength, int iRuleIndex, SHighlightIndexes *pHighlightIndexes);

};

#endif // CSYNTAXHIGHLIGHTER_H
