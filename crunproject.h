#ifndef CRUNPROJECT_H
#define CRUNPROJECT_H

#include <QObject>
class QTextEdit;

class CRunProject : public QObject
{
    Q_OBJECT
public:
    explicit CRunProject(QObject *parent = 0);
    void startRunProject(QStringList listCommand, QTextEdit *pLog);
signals:

public slots:

};

#endif // CRUNPROJECT_H
