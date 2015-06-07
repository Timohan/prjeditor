#ifndef CDIALOGFINDCOMPOBOX_H
#define CDIALOGFINDCOMPOBOX_H

#include <QComboBox>
#include <QKeyEvent>

class CDialogFindCompobox : public QComboBox
{
    Q_OBJECT
public:
    explicit CDialogFindCompobox(QWidget *parent = 0);

signals:
    void enterPressed();

public slots:

protected:
    virtual void keyPressEvent(QKeyEvent * e);

};

#endif // CDIALOGFINDCOMPOBOX_H
