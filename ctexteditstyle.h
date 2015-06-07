#ifndef CTEXTEDITSTYLE_H
#define CTEXTEDITSTYLE_H

#include <QObject>
#include <QStringList>
#include <QColor>

class CTextEditHighlightRule;
class CTextEditHighlightRules;

class CTextEditStyle : public QObject
{
    Q_OBJECT
public:
    explicit CTextEditStyle(QObject *parent, QString strStyleName);
    ~CTextEditStyle();
    
    QString getStyleName();
    void setStyleName(QString strStyleName);

    int getFileExtensionCount();
    QString getFileExtension(int i);
    void addFileExtension(QString strFileExtension);
    void removeFileExtension(QString strFileExtension);
    void removeFileExtension(int iIndex);
    int getFileExtensionIndex(QString strFileExtension);

    void addTextEditHighlightRule(CTextEditHighlightRule *pRule);

    int getTextEditHighlightRuleCount();
    CTextEditHighlightRule *getTextEditHighlightRule(int iIndex);

signals:
    
public slots:

private:
    QString m_strStyleName;
    QStringList m_listFileExtension;
    CTextEditHighlightRules *m_pTextEditHighlightRules;
};

#endif // CTEXTEDITSTYLE_H
