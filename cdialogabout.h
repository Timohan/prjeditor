#ifndef CDIALOGABOUT_H
#define CDIALOGABOUT_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>

class CDialogAbout : public QDialog
{
    Q_OBJECT
public:
    explicit CDialogAbout(QWidget *parent = 0);

signals:

public slots:
    void onCloseClicked();

protected:
    void resizeEvent(QResizeEvent *event);

private:
    QLabel *m_labelName;
    QLabel *m_labelCreator;
    QLabel *m_labelLicense;
    QPushButton *m_buttonClose;

    void refreshPosition();
};

#endif // CDIALOGABOUT_H
