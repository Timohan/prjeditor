#ifndef CABSTRACTTEXTDOCUMENTLAYOUT_H
#define CABSTRACTTEXTDOCUMENTLAYOUT_H

#include <QAbstractTextDocumentLayout>
#include <QTextDocument>

class CAbstractTextDocumentLayout : public QAbstractTextDocumentLayout
{
    Q_OBJECT
public:
    explicit CAbstractTextDocumentLayout(QTextDocument *parent = 0);

    void updateThisBlock(const QTextBlock &block);
    

signals:
    
public slots:
    
};

#endif // CABSTRACTTEXTDOCUMENTLAYOUT_H
