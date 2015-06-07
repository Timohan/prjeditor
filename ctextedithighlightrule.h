#ifndef CTEXTEDITHIGHLIGHTRULE_H
#define CTEXTEDITHIGHLIGHTRULE_H

#include <QObject>
#include <QStringList>
#include <QColor>

class CTextEditHighlightRule : public QObject
{
    Q_OBJECT
public:
    explicit CTextEditHighlightRule(QObject *parent = 0);

    enum TextEditStyleRuleType {
        TEXT_EDIT_STYLE_RULE_WORD,
        TEXT_EDIT_STYLE_RULE_WORD_REGEXP,
        TEXT_EDIT_STYLE_RULE_WORDS_BETWEEN,
        TEXT_EDIT_STYLE_RULE_WORDS_BETWEEN_SAME_LINE,
        TEXT_EDIT_STYLE_RULE_START_LINE_WITH_WORD,
        TEXT_EDIT_STYLE_RULE_ENLIGHT_ON_CURSOR,
    };
    void setRuleWord(QStringList listWord, QColor cColor);
    void setRuleRegExp(QStringList listWord, QColor cColor);

    void setRuleWordsBetween(QString strBegin, QString strEnd, QColor cColor, bool bSameLine = false, QStringList listIgnoredBegin = QStringList(), QStringList listIgnoredEnd = QStringList());
    void setRuleStartLineWithWord(QStringList listWord, QColor cColor);
    void setRuleEnlightOnCursor(QStringList listWord, QColor cColor);
    CTextEditHighlightRule::TextEditStyleRuleType getRuleType();
    QStringList getListWord();
    QColor getColor();
    QString getListWordInRegExp();
    QStringList getListIgnoredBegin();
    QStringList getListIgnoredEnd();

    int getPriorityIndex();
    void setPriorityIndex(int iPriorityIndex);

signals:
    
public slots:

private:
    TextEditStyleRuleType m_pType;
    QStringList m_listWord;
    QColor m_cColor;
    QString m_strListWordInRegExp;
    QStringList m_listIgnoredBegin;
    QStringList m_listIgnoredEnd;

    /** priority index of the rule, lower is better
     * @brief m_iPriorityIndex
     */
    int m_iPriorityIndex;


};

#endif // CTEXTEDITHIGHLIGHTRULE_H
