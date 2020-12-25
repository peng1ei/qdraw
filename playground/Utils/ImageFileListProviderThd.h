#pragma once
#include <QThread>
#include <QStringListModel>

class ImageFileListProviderThd : public QThread {
    Q_OBJECT
public:
    ImageFileListProviderThd(const QString &dirPath, const QStringList &filters);

    bool IsSucceed() { return mIsSucceed; }
    QStringList GetImageFileList() const { return mImageFileList; };

    void SetListMode(QStringListModel *model) { mModel = model; }

signals:
    void sigResult(bool, QString);

protected:
    void run() override;

private:
    bool mIsSucceed;
    QString mDirPath;
    QStringList mFilters;
    QStringList mImageFileList;
    QStringListModel *mModel = nullptr;
};

