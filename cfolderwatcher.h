#ifndef CFOLDERWATCHER_H
#define CFOLDERWATCHER_H

#include <QObject>

class CFolderWatcher : public QObject
{
    Q_OBJECT
public:
    explicit CFolderWatcher(QObject *parent = 0);
    bool watchFolder(QString strFolder);
signals:
    
public slots:
    
};

#endif // CFOLDERWATCHER_H
