#include "application.h"
#include "ctexteditstylelist.h"
#include "ctexteditstyle.h"
#include "cmainwindow.h"
#include "ctoolbarwidget.h"
#include "ctextedithighlightrule.h"
#include <QFile>
#include <QXmlStreamReader>
#include <QDebug>
#include <QDir>
#include <QDomDocument>

CTextEditStyleList::CTextEditStyleList(CMainWindow *parent) :
    QObject(parent),
    m_pMainWindow(parent)
{
    loadHighlights();
}
CTextEditStyleList::~CTextEditStyleList()
{
    int i;

    for (i=0;i<m_listTextStyle.size();i++)
    {
        delete m_listTextStyle[i];
        m_listTextStyle[i] = nullptr;
    }

    m_listTextStyle.clear();
}

void CTextEditStyleList::addTextStyleName(QString strStyleName)
{
    if (!getTextEditStyle(strStyleName))
    {
        m_listTextStyle.append(new CTextEditStyle(this, strStyleName));
        m_pMainWindow->getToolBarWidget()->addTextStyleName(strStyleName);
    }
}
void CTextEditStyleList::addFileExtensionInTextStyle(QString strFileExtension, QString strStyleName)
{
    CTextEditStyle *pStyle = getTextEditStyle(strStyleName);

    if (pStyle)
    {
        pStyle->addFileExtension(strFileExtension);
    }
}

CTextEditStyle *CTextEditStyleList::getTextEditStyle(int iIndex)
{
    return m_listTextStyle.at(iIndex);
}


CTextEditStyle *CTextEditStyleList::getTextEditStyleByFileExtension(QString strFileExtension)
{
    int i;

    for (i=0;i<m_listTextStyle.size();i++)
    {
        CTextEditStyle *pStyle = m_listTextStyle.at(i);
        if (pStyle->getFileExtensionIndex(strFileExtension) != -1)
        {
            return pStyle;
        }
    }

    return nullptr;
}


CTextEditStyle *CTextEditStyleList::getTextEditStyle(QString strStyleName)
{
    int i;

    for (i=0;i<m_listTextStyle.size();i++)
    {
        CTextEditStyle *pStyle = m_listTextStyle.at(i);
        if (pStyle->getStyleName() == strStyleName)
        {
            return pStyle;
        }
    }

    return nullptr;
}

bool CTextEditStyleList::loadHighlights() {

    QFile fileXml( HIGHLIGHT_FILEPATH );
    if (!fileXml.open(QIODevice::ReadOnly | QIODevice::Text)) {

        fileXml.setFileName("xml/highlights.xml");
        if (!fileXml.open(QIODevice::ReadOnly | QIODevice::Text)) {

            // no highlights - add one default highlight
            if (m_listTextStyle.size() == 0) {

                addTextStyleName(STRING_PLAIN_TEXT);
            }
            return false;
        }
    }

    QDomDocument mDocument;
    if (!mDocument.setContent(fileXml.readAll()))
    {
        // no highlights - add one default highlight
        if (m_listTextStyle.size() == 0) {

            addTextStyleName(STRING_PLAIN_TEXT);
        }

        return false;
    }
    QString strHighlightName;
    QString strColorR;
    QString strColorG;
    QString strColorB;
    int r,g,b;
    int i;
    QStringList listWord;
    QStringList listIgnoredWord;
    QDomElement pHighlightsElement = mDocument.firstChildElement();
    if (pHighlightsElement.nodeName() == "highlights") {

        QDomElement pHighlightElement = pHighlightsElement.firstChildElement();

        while ( !pHighlightElement.isNull() )
        {
            if (pHighlightElement.nodeName() == "highlight" &&
                pHighlightElement.attribute("name").isEmpty() == false) {

                // new highlight here
                strHighlightName = pHighlightElement.attribute("name");
                addTextStyleName(strHighlightName);
                CTextEditStyle *pStyle = getTextEditStyle(strHighlightName);

                QDomElement pExtensionOrRuleElement = pHighlightElement.firstChildElement();

                while ( !pExtensionOrRuleElement.isNull() )
                {

                    if (pExtensionOrRuleElement.nodeName() == "extension" &&
                        pExtensionOrRuleElement.text().isEmpty() == false) {

                        addFileExtensionInTextStyle(pExtensionOrRuleElement.text(), strHighlightName);
                    }

                    if (pExtensionOrRuleElement.nodeName() == "rule" &&
                        pExtensionOrRuleElement.attribute("type").isEmpty() == false) {

                        strColorR = pExtensionOrRuleElement.attribute("colorR");
                        strColorG = pExtensionOrRuleElement.attribute("colorG");
                        strColorB = pExtensionOrRuleElement.attribute("colorB");

                        if (strColorR.isEmpty() == false &&
                            strColorG.isEmpty() == false &&
                            strColorB.isEmpty() == false ) {

                            r = atoi(qPrintable(strColorR));
                            g = atoi(qPrintable(strColorG));
                            b = atoi(qPrintable(strColorB));
                            listWord.clear();
                            listIgnoredWord.clear();

                            QDomElement pWordElement = pExtensionOrRuleElement.firstChildElement();

                            while ( !pWordElement.isNull() )
                            {
                                if (pWordElement.nodeName() == "word") {

                                    listWord.append( pWordElement.text() );
                                }
                                else if (pWordElement.nodeName() == "ignoredword") {

                                    listIgnoredWord.append( pWordElement.text() );
                                }

                                pWordElement = pWordElement.nextSiblingElement();
                            }

                            if (listWord.size() == 2 &&
                                pExtensionOrRuleElement.attribute("type") == "RuleWordsBetween") {

                                if (listIgnoredWord.size() == 0) {

                                    CTextEditHighlightRule *pRule = new CTextEditHighlightRule(nullptr);
                                    pRule->setRuleWordsBetween(listWord.at(0), listWord.at(1), QColor(r, g, b));
                                    pStyle->addTextEditHighlightRule(pRule);
                                }
                                else {

                                    CTextEditHighlightRule *pRule = new CTextEditHighlightRule(nullptr);
                                    pRule->setRuleWordsBetween(listWord.at(0), listWord.at(1), QColor(r, g, b), true, listIgnoredWord, listIgnoredWord);
                                    pStyle->addTextEditHighlightRule(pRule);
                                }

                            }
                            else if (pExtensionOrRuleElement.attribute("type") == "RuleRegExp") {

                                for (i=0;i<listWord.size();i++) {
                                    CTextEditHighlightRule *pRule = new CTextEditHighlightRule(nullptr);
                                    QStringList l;
                                    l << listWord.at(i);
                                    pRule->setRuleRegExp(l, QColor(r, g, b));
                                    pStyle->addTextEditHighlightRule(pRule);
                                }
                            }
                            else if (pExtensionOrRuleElement.attribute("type") == "RuleSimpleWord") {

                                CTextEditHighlightRule *pRule = new CTextEditHighlightRule(nullptr);
                                pRule->setRuleWord(listWord, QColor(r, g, b));
                                pStyle->addTextEditHighlightRule(pRule);
                            }
                            else if (pExtensionOrRuleElement.attribute("type") == "RuleEnlightOnCursor") {

                                CTextEditHighlightRule *pRule = new CTextEditHighlightRule(nullptr);
                                pRule->setRuleEnlightOnCursor(listWord, QColor(r, g, b));
                                pStyle->addTextEditHighlightRule(pRule);
                            }
                        }
                    }

                    pExtensionOrRuleElement = pExtensionOrRuleElement.nextSiblingElement();
                }
            }

            pHighlightElement = pHighlightElement.nextSiblingElement();
        }
    }
    // no highlights - add one default highlight
    if (m_listTextStyle.size() == 0) {

        addTextStyleName(STRING_PLAIN_TEXT);
    }

    fileXml.close();

    return true;
}
