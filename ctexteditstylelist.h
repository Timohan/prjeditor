#ifndef CTEXTEDITSTYLELIST_H
#define CTEXTEDITSTYLELIST_H

#include <QObject>
class CTextEditStyle;
class CMainWindow;

class CTextEditStyleList : public QObject
{
    Q_OBJECT
public:
    explicit CTextEditStyleList(CMainWindow *parent = 0);
    ~CTextEditStyleList();
    
    void addTextStyleName(QString strStyleName);
    void addFileExtensionInTextStyle(QString strFileExtension, QString strStyleName);

    CTextEditStyle *getTextEditStyle(QString strStyleName);
    CTextEditStyle *getTextEditStyleByFileExtension(QString strFileExtension);

    CTextEditStyle *getTextEditStyle(int iIndex);
signals:
    
public slots:

private:
    QList<CTextEditStyle *>m_listTextStyle;

    /**
     * @brief m_pMainWindow
     * pointer to main window
     */
    CMainWindow *m_pMainWindow;


    bool loadHighlights();
    
};

#endif // CTEXTEDITSTYLELIST_H
