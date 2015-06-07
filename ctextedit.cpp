#include "ctextedit.h"
#include "ctexteditstyle.h"
#include "cabstracttextdocumentlayout.h"
#include "csyntaxhighlighter.h"
#include "cmainwindow.h"
#include "cdialogfind.h"
#include "ctoolbarwidget.h"
#include <QDebug>
#include <QMimeData>
#include <QAbstractTextDocumentLayout>
#include <QTextDocument>

CTextEdit::CTextEdit(QWidget *parent, CMainWindow *pMainWindow, QLabel *labelCursorPosition) :
    QTextEdit(parent),
    m_pMainWindow(pMainWindow),
    m_pStyle(NULL),
    m_labelCursorPosition(labelCursorPosition),
    m_pTextDocument(NULL),
    m_pSyntaxHighlighter(NULL)
{
    setStyleSheet("font: 9pt \"Monospace\";");
    setLineWrapMode(QTextEdit::NoWrap);

    m_pSearchStartType.m_eSearchNext = SEARCH_START_TYPE_NOTHING;
    m_pSearchStartType.m_iStartPosition = 0;

    m_pTextDocument = new QTextDocument(this);
    setDocument(m_pTextDocument);
    m_pSyntaxHighlighter = new CSyntaxHighlighter(m_pTextDocument, this, pMainWindow);
    connect(this, SIGNAL(cursorPositionChanged()), SLOT(cursorPositionChanged()) );
}

void CTextEdit::setPlainText(const QString &text)
{
    m_strLastSavedText = text;
    QTextEdit::setPlainText(text);
}

void CTextEdit::setText(const QString &text)
{
    m_strLastSavedText = text;
    QTextEdit::setText(text);
}

QString CTextEdit::getLastSavedText()
{
    return m_strLastSavedText;
}

void CTextEdit::setLastSavedText(QString text)
{
    m_strLastSavedText = text;
}

void CTextEdit::setTextEditStyle(CTextEditStyle *style)
{
    if (m_pStyle == NULL || style == NULL ||
       m_pStyle->getStyleName() != style->getStyleName())
    {
        m_pStyle = style;
        m_pSyntaxHighlighter->setTextEditRules(style);
        m_pSyntaxHighlighter->rehighlight();
    }
}
CTextEditStyle *CTextEdit::getStyle()
{
    return m_pStyle;
}

void CTextEdit::clearHighlightedOnCursorBlock()
{
    int i;
    m_pSyntaxHighlighter->setHighlightOnCursorMode(CSyntaxHighlighter::SYNTAXHIGHLIGHTER_ON_CURSOR_MODE_REMOVE);
    for (i=0;i<m_pSyntaxHighlighter->getHighlightedOnCursorBlockCount();i++)
    {
        m_pSyntaxHighlighter->rehighlightBlock( m_pSyntaxHighlighter->getHighlightedOnCursorBlock(i) );
    }
    m_pSyntaxHighlighter->clearHighlightedOnCursorBlock();
}
/**
 * @brief CTextEdit::insertFromMimeData
 * when ctrl-v is pressed, this function is called
 * @param source
 */
void CTextEdit::insertFromMimeData(const QMimeData *source)
{
    if (source->hasText() || source->hasHtml())
    {
        QMimeData mimeData;
        mimeData.setText(source->text());
        QTextEdit::insertFromMimeData(&mimeData);
    }
}

void CTextEdit::cursorPositionChanged()
{
    QTextCursor cursor = textCursor();
    QTextBlock block = cursor.block();
    block.blockFormatIndex();

    m_labelCursorPosition->setText(QString("%1 : %2").arg(1+block.firstLineNumber()).arg( cursor.selectionStart() - block.position() ) );

    clearHighlightedOnCursorBlock();

    m_pSyntaxHighlighter->setHighlightOnCursorMode(CSyntaxHighlighter::SYNTAXHIGHLIGHTER_ON_CURSOR_MODE_NORMAL);
    m_pSyntaxHighlighter->rehighlightBlock(block);

    m_pSyntaxHighlighter->setHighlightOnCursorMode(CSyntaxHighlighter::SYNTAXHIGHLIGHTER_ON_CURSOR_MODE_ADD);

    while (1)
    {
        if (m_pSyntaxHighlighter->getBlockHighlightedOnCursorIndex() == 0)
        {
            break;
        }

        if (m_pSyntaxHighlighter->getBlockHighlightedOnCursorIndex() > 0)
        {
            block = block.next();
        }
        else
        {
            block = block.previous();
        }

        if (!block.isValid())
        {
            clearHighlightedOnCursorBlock();
            break;
        }
        if (block.text().indexOf(m_pSyntaxHighlighter->getHighlightOnCusorStart()) != -1 ||
            block.text().indexOf(m_pSyntaxHighlighter->getHighlightOnCusorEnd()) != -1)
        {
            m_pSyntaxHighlighter->rehighlightBlock(block);
        }
    }

    m_pSyntaxHighlighter->setHighlightOnCursorMode(CSyntaxHighlighter::SYNTAXHIGHLIGHTER_ON_CURSOR_MODE_NONE);
}


CTextEdit::ESearchResult CTextEdit::searchNext(QString strText, Qt::CaseSensitivity caseSensitivity,
                                               bool bNewSearch, int iSearchPlus)
{
    QTextCursor cursor;
    cursor = textCursor();

    int iToPosition;

    if (bNewSearch || m_pSearchStartType.m_eSearchNext != SEARCH_START_TYPE_NEXT)
    {
        m_pSearchStartType.m_eSearchNext = SEARCH_START_TYPE_NEXT;
        m_pSearchStartType.m_iStartPosition = cursor.selectionStart();
        bNewSearch = true;
    }

    if (bNewSearch == false)
    {
        iToPosition = this->toPlainText().indexOf(strText, cursor.selectionStart()+1, caseSensitivity);
    }
    else
    {
        iToPosition = this->toPlainText().indexOf(strText, cursor.selectionStart()+iSearchPlus, caseSensitivity);
    }

    if (iToPosition != -1)
    {
        if (bNewSearch == false &&
            (cursor.selectionStart() == iToPosition ||
             (m_pSearchStartType.m_iStartPosition >= cursor.position() && m_pSearchStartType.m_iStartPosition <= iToPosition)))
        {
            return SEARCH_RESULT_ALL_FOUND_ALREADY;
        }

        cursor.setPosition(iToPosition);
        cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, strText.size() );
        this->setTextCursor(cursor);
        return SEARCH_RESULT_OK;
    }

    if (bNewSearch == false && cursor.selectionStart() == 0)
    {
        iToPosition = this->toPlainText().indexOf(strText, 1, caseSensitivity);
    }
    else
    {
        iToPosition = this->toPlainText().indexOf(strText, iSearchPlus, caseSensitivity);
    }

    if (iToPosition != -1)
    {
        if (bNewSearch == false &&
            (cursor.selectionStart() == iToPosition ||
             m_pSearchStartType.m_iStartPosition <= iToPosition))
        {
            return SEARCH_RESULT_ALL_FOUND_ALREADY;
        }

        cursor.setPosition(iToPosition);
        cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, strText.size() );
        this->setTextCursor(cursor);
        return SEARCH_RESULT_OK;
    }

    return SEARCH_RESULT_NOT_FOUND;
}

CTextEdit::ESearchResult CTextEdit::searchPrevious(QString strText, Qt::CaseSensitivity caseSensitivity, bool bNewSearch)
{
    QTextCursor cursor;
    cursor = textCursor();

    int iToPosition = -1;

    iToPosition = this->toPlainText().lastIndexOf(strText, cursor.position()-this->toPlainText().size()-strText.size()-1, caseSensitivity);

    if (bNewSearch || m_pSearchStartType.m_eSearchNext != SEARCH_START_TYPE_PREVIOUS)
    {
        m_pSearchStartType.m_eSearchNext = SEARCH_START_TYPE_PREVIOUS;
        m_pSearchStartType.m_iStartPosition = cursor.position();
        bNewSearch = true;
    }

    if (iToPosition != -1)
    {
        if (bNewSearch == false &&
            (cursor.position() == iToPosition ||
             (m_pSearchStartType.m_iStartPosition <= cursor.position() && m_pSearchStartType.m_iStartPosition >= iToPosition)))
        {
            return SEARCH_RESULT_ALL_FOUND_ALREADY;
        }

        cursor.setPosition(iToPosition);
        cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, strText.size() );
        this->setTextCursor(cursor);
        return SEARCH_RESULT_OK;
    }
    iToPosition = this->toPlainText().lastIndexOf(strText, -1, caseSensitivity);

    if (iToPosition != -1)
    {
        if (bNewSearch == false &&
            (cursor.position() == iToPosition ||
             m_pSearchStartType.m_iStartPosition >= iToPosition))
        {
            return SEARCH_RESULT_ALL_FOUND_ALREADY;
        }

        cursor.setPosition(iToPosition);
        cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, strText.size() );
        this->setTextCursor(cursor);
        return SEARCH_RESULT_OK;
    }

    return SEARCH_RESULT_NOT_FOUND;
}

CTextEdit::ESearchResult CTextEdit::replace()
{
    int iPreviousPosition = 0;
    int iPosition;
    int iCursorPosition;
    bool bFound = false;
    CDialogFind *pDialogFind = m_pMainWindow->getDialogFind();
    QString strTextToFind = pDialogFind->getFindText();
    Qt::CaseSensitivity caseSensitivity = pDialogFind->getCaseSensitivity();
    QTextCursor cursor;
    CDialogFind::EDIALOG_FIND_SEARCH_TYPE type = pDialogFind->getSearchType();
    QString strReplaceText = pDialogFind->getFindToReplace();

    cursor = textCursor();
    int iStart, iEnd;
    iCursorPosition = cursor.position();

    if (type == CDialogFind::EDIALOG_FIND_SEARCH_TYPE_IN_SELECTION) {

        if (pDialogFind->getReplaceAll()) {

            iPreviousPosition = cursor.selectionStart();
            iEnd = cursor.selectionEnd();
            while (1)
            {
                iPosition = this->toPlainText().indexOf(strTextToFind, iPreviousPosition, caseSensitivity);

                if (iPosition + strTextToFind.size() <= iEnd)
                {
                    bFound = true;
                    cursor.setPosition(iPosition);
                    cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, strTextToFind.size() );
                    this->setTextCursor(cursor);
                    this->textCursor().insertText(strReplaceText);

                    if (iPosition < iCursorPosition) {

                        iCursorPosition += strReplaceText.size() - strTextToFind.size();
                    }

                    iPreviousPosition = iPosition + strReplaceText.size();
                    iEnd += strReplaceText.size() - strTextToFind.size();
                }
                else
                {
                    if (bFound)
                    {
                        cursor.setPosition(iCursorPosition);
                        this->setTextCursor(cursor);

                        return SEARCH_RESULT_OK;
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }
        else
        {
            iStart = cursor.selectionStart();
            iEnd = cursor.selectionEnd();
            iPosition = this->toPlainText().indexOf(strTextToFind, cursor.selectionStart(), caseSensitivity);
            if (iPosition >= iStart &&
                iPosition + strTextToFind.size() <= iEnd)
            {
                cursor.setPosition(iPosition);
                cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, strTextToFind.size() );
                this->setTextCursor(cursor);
                this->textCursor().insertText(strReplaceText);

                cursor.setPosition(iStart);
                cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, iEnd - iStart + strReplaceText.size() - strTextToFind.size() );
                this->setTextCursor(cursor);

                return SEARCH_RESULT_OK;
            }
        }
    }
    else {
        if (pDialogFind->getReplaceAll()) {
            while (1)
            {
                iPosition = this->toPlainText().indexOf(strTextToFind, iPreviousPosition, caseSensitivity);

                if (iPosition == -1)
                {
                    if (bFound)
                    {
                        cursor.setPosition(iCursorPosition);
                        this->setTextCursor(cursor);
                        return SEARCH_RESULT_OK;
                    }
                    else
                    {
                        break;
                    }
                }

                bFound = true;
                if (iPosition < iCursorPosition) {

                    iCursorPosition += strReplaceText.size() - strTextToFind.size();
                }
                cursor.setPosition(iPosition);
                cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, strTextToFind.size() );
                this->setTextCursor(cursor);
                this->textCursor().insertText(strReplaceText);
                iPreviousPosition = iPosition + strReplaceText.size();
            }
        }
        else {
            iPosition = this->toPlainText().indexOf(strTextToFind, cursor.selectionStart(), caseSensitivity);

            if (iPosition == -1) {

                iPosition = this->toPlainText().indexOf(strTextToFind, 0, caseSensitivity);
            }

            if (iPosition != -1) {

                cursor.setPosition(iPosition);
                cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, strTextToFind.size() );
                this->setTextCursor(cursor);
                this->textCursor().insertText(strReplaceText);

                return SEARCH_RESULT_OK;
            }
        }
    }

    return SEARCH_RESULT_NOT_FOUND;
}

void CTextEdit::setCursorPosition(int iLineNumber, QString strToText)
{
    int iTextPosition = 0;
    int iNewCursorPosition = 0;
    int iTextToPosition = 0;
    QTextCursor cursor;
    int iCmpLineNumber = 0;
    cursor = textCursor();

    if (iLineNumber > 0)
    {
        while (1)
        {
            iTextToPosition = this->toPlainText().indexOf("\n", iTextPosition);
            if (iTextToPosition == -1)
            {
                iNewCursorPosition = this->toPlainText().size();
                break;
            }

            iCmpLineNumber++;

            if (iLineNumber == iCmpLineNumber)
            {
                iNewCursorPosition = iTextToPosition + 1;
                break;
            }

            iTextPosition = iTextToPosition + 1;
        }
    }


    iTextToPosition = this->toPlainText().indexOf(strToText, iNewCursorPosition);

    if (iTextToPosition == -1)
    {
        cursor.setPosition(iNewCursorPosition);
    }
    else
    {
        cursor.setPosition(iTextToPosition);
        cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, strToText.size() );
    }

    this->setTextCursor(cursor);
    this->setFocus(Qt::OtherFocusReason);
}

void CTextEdit::keyPressEvent(QKeyEvent * e)
{
    if (e->key() == Qt::Key_Tab &&
        (m_pMainWindow->getToolBarWidget()->getTabulatorIndex() == 0 ||
         m_pMainWindow->getToolBarWidget()->getTabulatorIndex() == 1))
    {
        int iStep = 4;
        if (m_pMainWindow->getToolBarWidget()->getTabulatorIndex() == 1) {

            iStep = 2;
        }
        int i;
        QTextCursor cursor;
        cursor = textCursor();
        QTextBlock block = cursor.block();
        int iCursorPositionOnLine = cursor.selectionStart() - block.position();
        int iTabCount = iCursorPositionOnLine/iStep;
        int iNewSpaceCount = iCursorPositionOnLine - iTabCount*iStep;
        if (iCursorPositionOnLine == iTabCount*iStep)
        {
            iNewSpaceCount = iStep;
        }
        else
        {
            iNewSpaceCount = iTabCount*iStep + iStep - iCursorPositionOnLine;
        }

        for (i=0;i<iNewSpaceCount;i++)
        {
            this->textCursor().insertText(" ");
        }
    }
    else
    {
        QTextEdit::keyPressEvent(e);
    }
}
