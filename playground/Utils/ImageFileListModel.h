#ifndef IMAGEFILELISTMODEL_H
#define IMAGEFILELISTMODEL_H
#include <QStringListModel>
#include <QFile>
#include <QTextStream>
#include <QColor>
#include <QSet>

// 参考：https://www.walletfox.com/course/qtcheckablelist.php
// https://stackoverflow.com/questions/37781426/how-to-change-the-color-of-qstringlistmodel-items
class ImageFileListModel : public QStringListModel
{
public:
    ImageFileListModel(QObject* parent = 0);
    ImageFileListModel(const QStringList & strings, QObject* parent = 0);
    Qt::ItemFlags flags (const QModelIndex& index) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role);
    void save();
private:
    QSet<QPersistentModelIndex> checkedItems;
};
#endif // IMAGEFILELISTMODEL_H
