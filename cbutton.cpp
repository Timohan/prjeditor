#include "cbutton.h"
#include "QApplication"
#include <QDebug>
CButton::CButton(QWidget *parent, QString strToolTip, QString strFilePathImage) :
    QPushButton(parent)
{
    this->setToolTip(strToolTip);
    if (strFilePathImage.size() != 0)
    {
        QPixmap pixmap(":/images/" + strFilePathImage);
        this->setIcon(pixmap);
    }
}
