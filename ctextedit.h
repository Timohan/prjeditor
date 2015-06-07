#ifndef CTEXTEDIT_H
#define CTEXTEDIT_H

#include <QTextEdit>
#include <QLabel>
class CTextEditStyle;
class QTextDocument;
class CSyntaxHighlighter;
class CAbstractTextDocumentLayout;
class CMainWindow;

enum ETextSearchStartType
{
    SEARCH_START_TYPE_NOTHING,
    SEARCH_START_TYPE_NEXT,
    SEARCH_START_TYPE_PREVIOUS
};

struct STextSearchStartType
{
    /**
     * @brief m_eSearchNext
     * SEARCH_START_TYPE_NOTHING == no searching
     * SEARCH_START_TYPE_NEXT == search next
     * SEARCH_START_TYPE_PREVIOUS == search previous
     */
    ETextSearchStartType m_eSearchNext;
    /**
     * @brief m_iStartPosition
     * cursor position when search did begin
     */
    int m_iStartPosition;
};

class CTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    enum ESearchResult {
        SEARCH_RESULT_OK,
        SEARCH_RESULT_NOT_FOUND,
        SEARCH_RESULT_ALL_FOUND_ALREADY
    };

    explicit CTextEdit(QWidget *parent, CMainWindow *pMainWindow, QLabel *labelCursorPosition);

    void setTextEditStyle(CTextEditStyle *style);

    QString getLastSavedText();
    void setLastSavedText(QString text);

    CTextEditStyle *getStyle();

    ESearchResult searchNext(QString strText, Qt::CaseSensitivity caseSensitivity, bool bNewSearch, int iSearchPlus = 0);
    ESearchResult searchPrevious(QString strText, Qt::CaseSensitivity caseSensitivity, bool bNewSearch);
    ESearchResult replace();

    void setCursorPosition(int iLineNumber, QString strToText);

signals:
    
public slots:
    void cursorPositionChanged();

    void setPlainText(const QString &text);
    void setText(const QString &text);

protected:
    virtual void insertFromMimeData(const QMimeData *source);
    virtual void keyPressEvent(QKeyEvent *e);

private:
    CMainWindow *m_pMainWindow;
    CTextEditStyle *m_pStyle;
    QLabel *m_labelCursorPosition;
    STextSearchStartType m_pSearchStartType;
    
    QString m_strLastSavedText;

    QTextDocument *m_pTextDocument;
    CSyntaxHighlighter *m_pSyntaxHighlighter;
    CAbstractTextDocumentLayout *m_pTextDocumentLayout;

    void clearHighlightedOnCursorBlock();


};

#endif // CTEXTEDIT_H
