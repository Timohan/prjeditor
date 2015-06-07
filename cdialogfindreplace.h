#ifndef CDIALOGFINDREPLACE_H
#define CDIALOGFINDREPLACE_H

#include <QWidget>
class QLabel;
class QComboBox;
class QSettings;
class CDialogFindCompobox;

class CDialogFindReplace : public QWidget
{
    Q_OBJECT
public:
    explicit CDialogFindReplace(QWidget *parent = 0);
    
    QString getFindToReplace();

    bool isChangedEnoughToSave();
    void loadValuesFromSettings(QSettings *pSettings);
    void saveValuesToSettings(QSettings *pSettings);

signals:
    
public slots:

private:
    QLabel *m_labelReplace;
    CDialogFindCompobox *m_comboboxReplace;

};

#endif // CDIALOGFINDREPLACE_H
