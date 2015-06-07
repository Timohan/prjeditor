#ifndef CBUTTON_H
#define CBUTTON_H

#include <QPushButton>

class CButton : public QPushButton
{
    Q_OBJECT
public:
    enum BUTTON_TYPE
    {
        BUTTON_TYPE_TOOLBAR
    };

    explicit CButton(QWidget *parent, QString strToolTip, QString strFilePathImage);

signals:
    
public slots:
    
};

#endif // CBUTTON_H
