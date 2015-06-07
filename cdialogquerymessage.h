#ifndef CDIALOGQUERYMESSAGE_H
#define CDIALOGQUERYMESSAGE_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

class CDialogLineEditBase;

class CDialogQueryMessage : public QDialog
{
    Q_OBJECT
public:
    explicit CDialogQueryMessage(QWidget *parent = 0);
    
    void setDialog(QString strQuestion, QString strPlaceholder, QString strDefaultText = "");
    QString getAnswer();
signals:
    
public slots:
    void onCancelClicked();
    void onYesClicked();

protected:
    virtual void resizeEvent(QResizeEvent *event);

private:
    QLabel *m_labelQuestion;
    CDialogLineEditBase *m_lineQuestion;

    QPushButton *m_buttonCancel;
    QPushButton *m_buttonYes;

    
};

#endif // CDIALOGQUERYMESSAGE_H
