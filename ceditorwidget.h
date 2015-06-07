#ifndef CEDITORWIDGET_H
#define CEDITORWIDGET_H

#include <QWidget>
class CTextEdit;
class CMainWindow;
class CEditorTabWidget;
class CTextEditStyle;
class CRunProject;
class QLabel;

class CEditorWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CEditorWidget(CMainWindow *pMainWindow, CEditorTabWidget *parent, QString strFileName);
    ~CEditorWidget();

    QString getFileName();
    void setFileName(QString strFileName);

    qint64 getId();
    void setId(qint64 iId);

    bool isTextSameAsSaved();

    QString getStyleName();
    void setTextEditStyle(CTextEditStyle *style);

    bool save(bool bToNewFile = false);

    CTextEdit *getTextEdit();

    QString getSelectedText() const;

signals:
    
public slots:
    void textChanged();

protected:
    virtual void resizeEvent(QResizeEvent *e);

private:
    QLabel *m_labelCursorPosition;
    CEditorTabWidget *m_pEditorTabWidget;
    QString m_strFileName;
    CTextEdit *m_pTextEdit;
    qint64 m_iId;

    /**
     * @brief m_pMainWindow
     * pointer to main window
     */
    CMainWindow *m_pMainWindow;

    CRunProject *m_pRunProject;

};

#endif // CEDITORWIDGET_H
