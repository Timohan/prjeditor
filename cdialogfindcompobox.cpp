#include <QDebug>
#include <QCompleter>
#include "cdialogfindcompobox.h"

CDialogFindCompobox::CDialogFindCompobox(QWidget *parent) :
    QComboBox(parent)
{
    setAutoCompletion(false);
}

void CDialogFindCompobox::keyPressEvent(QKeyEvent * e)
{
    if (e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return)
    {
        emit enterPressed();
    }
    else
    {
        QComboBox::keyPressEvent(e);
    }
}
