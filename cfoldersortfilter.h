#ifndef CFOLDERSORTFILTER_H
#define CFOLDERSORTFILTER_H

#include <QSortFilterProxyModel>

class CFolderSortFilter : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit CFolderSortFilter(QObject *parent = 0);
    
signals:
    
public slots:

protected:
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const;

private:
    QString getWindowsHardDiskDrive(const QModelIndex &index) const;
    
};

#endif // CFOLDERSORTFILTER_H
