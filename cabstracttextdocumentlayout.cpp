#include "cabstracttextdocumentlayout.h"

CAbstractTextDocumentLayout::CAbstractTextDocumentLayout(QTextDocument *parent) :
    QAbstractTextDocumentLayout(parent)
{
}

void CAbstractTextDocumentLayout::updateThisBlock(const QTextBlock &block)
{
    emit updateBlock(block);
}
