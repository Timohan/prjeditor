#include "application.h"
#include "cdialogquerymessage.h"
#include "cdialoglineeditbase.h"

CDialogQueryMessage::CDialogQueryMessage(QWidget *parent) :
    QDialog(parent)
{
    m_labelQuestion = new QLabel(this);
    m_lineQuestion = new CDialogLineEditBase(this);

    m_buttonCancel = new QPushButton(STRING_CANCEL, this);
    m_buttonYes = new QPushButton(STRING_CREATE, this);

    this->setMaximumSize(300,120);
    this->setMinimumSize(300,120);

    connect(m_buttonCancel, SIGNAL(clicked()), this, SLOT(onCancelClicked()));
    connect(m_buttonYes, SIGNAL(clicked()), this, SLOT(onYesClicked()));
    connect(m_lineQuestion, SIGNAL(onEnterPressed()), this, SLOT(onYesClicked()));
}


void CDialogQueryMessage::setDialog(QString strQuestion, QString strPlaceholder, QString strDefaultText)
{
    m_labelQuestion->setText(strQuestion);
    m_lineQuestion->setPlaceholderText(strPlaceholder);
    m_lineQuestion->setText(strDefaultText);
}

QString CDialogQueryMessage::getAnswer()
{
    return m_lineQuestion->text();
}

void CDialogQueryMessage::resizeEvent(QResizeEvent *e)
{
    QDialog::resizeEvent(e);

    const int iButtonHeight = 20;
    const int iButtonWidth = 70;

    m_labelQuestion->setGeometry(5,5,
                                 width()-10,
                                 m_labelQuestion->height());
    m_lineQuestion->setGeometry(5,10+m_labelQuestion->y()+m_labelQuestion->height(),
                                this->width()-10,
                                m_lineQuestion->height() );

    m_buttonYes->setGeometry(this->width()-5-iButtonWidth,
                             this->height()-5-iButtonHeight,
                             iButtonWidth, iButtonHeight);
    m_buttonCancel->setGeometry(m_buttonYes->x()-5-iButtonWidth,
                                m_buttonYes->y(),
                                iButtonWidth, iButtonHeight);
}

void CDialogQueryMessage::onCancelClicked()
{
    this->done(0);
}

void CDialogQueryMessage::onYesClicked()
{
    this->done(1);
}
