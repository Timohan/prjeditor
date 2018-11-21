#ifndef CTEXTEDITHIGHLIGHTRULES_H
#define CTEXTEDITHIGHLIGHTRULES_H

#include <QObject>

class CTextEditHighlightRule;
class CTextEditStyle;

class CTextEditHighlightRules: public QObject
{
    Q_OBJECT
public:
    CTextEditHighlightRules(QObject *parent = nullptr);
    ~CTextEditHighlightRules();

    void addTextEditHighlightRule(CTextEditHighlightRule *pRule);
    int getTextEditHighlightRuleCount();
    CTextEditHighlightRule *getTextEditHighlightRule(int iIndex);
private:
    QList<CTextEditHighlightRule *>m_pTextEditHighlightRule;


};

#endif // CTEXTEDITHIGHLIGHTRULES_H
