#include "cmainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    CMainWindow w;
    QCoreApplication::setAttribute(Qt::AA_DontUseNativeMenuBar);
    w.show();
    
    return a.exec();
}
