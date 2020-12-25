#include "ImageFileListProviderThd.h"
#include <QDir>
#include <QFileInfo>
#include <QDebug>

ImageFileListProviderThd::ImageFileListProviderThd(const QString &dirPath, const QStringList &filters)
    : mIsSucceed(false), mDirPath(dirPath), mFilters(filters) {

}

void ImageFileListProviderThd::run()
{
    QDir dir(mDirPath);
    QStringList filter(mFilters);

#if 1
    auto filenames = dir.entryList(mFilters);
    for (const auto &name : filenames) {
        mImageFileList.append(mDirPath + "/" + name);
    }

    if (mModel)
        mModel->setStringList(mImageFileList);
#else
    QList<QFileInfo> fileInfo(dir.entryInfoList(filter));
    if (fileInfo.empty()) {
        emit sigResult(false, tr("Failed to obtain image file!"));
        mIsSucceed = false;
        return;
    }

    for(int i = 0; i< fileInfo.count(); i++) {
        qDebug() << fileInfo.at(i).filePath();
        mImageFileList.append(fileInfo.at(i).filePath());
    }
#endif

    mIsSucceed = true;
    emit sigResult(true, tr("Open folder successfully!"));
}
