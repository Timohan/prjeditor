#include "cdialogabout.h"
#include "application.h"

CDialogAbout::CDialogAbout(QWidget *parent) :
    QDialog(parent)
{

    m_labelName = new QLabel(STRING_APPLICATION_NAME + " " + STRING_VERSION, this);
    m_labelCreator = new QLabel(STRING_CREATOR + STRING_ORGANIZATION, this);
    m_labelLicense = new QLabel(STRING_LICENSE, this);
    m_buttonClose = new QPushButton(STRING_CLOSE, this);

    this->setWindowTitle(STRING_ABOUT + " " + STRING_APPLICATION_NAME);

    connect(m_buttonClose, SIGNAL(clicked()), this, SLOT(onCloseClicked()));
}

void CDialogAbout::onCloseClicked() {

    this->close();
}

void CDialogAbout::resizeEvent(QResizeEvent *event)
{
    if (event)
    {
        QDialog::resizeEvent(event);
    }

    refreshPosition();
}

void CDialogAbout::refreshPosition() {

    this->setMinimumWidth(DEFAULT_ABOUT_TEXT_WIDTH+DEFAULT_MARGIN*2);
    this->setMaximumWidth(DEFAULT_ABOUT_TEXT_WIDTH+DEFAULT_MARGIN*2);

    m_labelName->setGeometry(DEFAULT_MARGIN, DEFAULT_MARGIN,
                            DEFAULT_ABOUT_TEXT_WIDTH,
                            m_labelName->height());
    m_labelCreator->setGeometry(DEFAULT_MARGIN, DEFAULT_MARGIN+m_labelName->y()+m_labelName->height(),
                            DEFAULT_ABOUT_TEXT_WIDTH,
                            m_labelCreator->height());
    m_labelLicense->setGeometry(DEFAULT_MARGIN, DEFAULT_MARGIN+m_labelCreator->y()+m_labelCreator->height(),
                            DEFAULT_ABOUT_TEXT_WIDTH,
                            m_labelLicense->height());
    m_buttonClose->setGeometry(this->width()-DEFAULT_MARGIN-m_buttonClose->width(),
                               DEFAULT_MARGIN+m_labelLicense->y()+m_labelLicense->height(),
                            m_buttonClose->width(),
                            m_buttonClose->height());

    this->setMinimumHeight(m_buttonClose->y() + m_buttonClose->height()+DEFAULT_MARGIN);
    this->setMaximumWidth(m_buttonClose->y() + m_buttonClose->height()+DEFAULT_MARGIN);}
