#include "cdialoglineeditbase.h"

CDialogLineEditBase::CDialogLineEditBase(QWidget *parent) :
    QLineEdit(parent)
{
}
void CDialogLineEditBase::keyPressEvent(QKeyEvent * e)
{
    if (e->key() == Qt::Key_Enter ||
        e->key() == Qt::Key_Return)
    {

        emit onEnterPressed();
    }
    else
    {
        QLineEdit::keyPressEvent(e);
    }
}
