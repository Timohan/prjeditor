#include "crunproject.h"
#include <stdio.h>
#include <QDebug>
#include <QTextEdit>
#include <QProcess>
CRunProject::CRunProject(QObject *parent) :
    QObject(parent)
{
}

void CRunProject::startRunProject(QStringList listCommand, QTextEdit *pLog)
{
    int i;
    QProcess process;
    QString result_all = "";

    for (i=0;i<listCommand.size();i++) {

        result_all += "\n $ " + listCommand.at(i) + "\n";
        pLog->setText(result_all);
        process.start( listCommand.at(i) );
        process.waitForFinished();
        result_all += process.readAllStandardOutput();
        pLog->setText(result_all);
    }
}
