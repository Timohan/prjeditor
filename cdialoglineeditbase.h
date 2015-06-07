#ifndef CDIALOGLINEEDITBASE_H
#define CDIALOGLINEEDITBASE_H

#include <QLineEdit>
#include <QKeyEvent>

class CDialogLineEditBase : public QLineEdit
{
    Q_OBJECT
public:
    explicit CDialogLineEditBase(QWidget *parent = 0);

signals:
    void onEnterPressed();

public slots:

protected:
    virtual void keyPressEvent(QKeyEvent * e);

};

#endif // CDIALOGLINEEDITBASE_H
