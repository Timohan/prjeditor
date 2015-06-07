#include "cfoldersortfilter.h"
#include <QDebug>
#include <QFileSystemModel>

CFolderSortFilter::CFolderSortFilter(QObject *parent) :
    QSortFilterProxyModel(parent)
{
}

QString CFolderSortFilter::getWindowsHardDiskDrive(const QModelIndex &index) const
{
    QModelIndex i = index;
    int iLastIndex;
    QString strDriveLabel;
    QVariant data;

    while (1)
    {
        if (i.parent() == QModelIndex())
        {
            data = sourceModel()->data(i);
            strDriveLabel = data.toString();

            iLastIndex = strDriveLabel.lastIndexOf(":");
            if (iLastIndex <= 0)
            {
                iLastIndex = 1;
            }
            strDriveLabel = strDriveLabel.right(strDriveLabel.size() - (iLastIndex-1));
            strDriveLabel.resize(1);
            return strDriveLabel;
        }

        i = i.parent();
    }
}

bool CFolderSortFilter::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
#ifdef WIN32
    if (left.parent() == QModelIndex()) // Hard disk partition of Windows
    {
        QString strLeftString = getWindowsHardDiskDrive(left);
        QString strRightString = getWindowsHardDiskDrive(right);
        return strLeftString > strRightString;
    }
#endif

    QVariant leftData = sourceModel()->data(left);
    QVariant rightData = sourceModel()->data(right);
    QFileSystemModel *fileModel = (QFileSystemModel *)sourceModel();
    if (fileModel->isDir(left) != fileModel->isDir(right))
    {
        if (fileModel->isDir(left) == true)
        {
            return false;
        }

        return true;
    }

    return leftData.toString() > rightData.toString();
}
