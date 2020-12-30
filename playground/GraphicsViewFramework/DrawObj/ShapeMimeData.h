#pragma once
#include <QMimeData>
#include <QList>

QT_BEGIN_NAMESPACE
class QGraphicsItem;
QT_END_NAMESPACE

namespace gvf {

class ShapeMimeData : public QMimeData
{
    Q_OBJECT
public:
    ShapeMimeData( QList<QGraphicsItem * > items);
    ~ShapeMimeData();
    QList<QGraphicsItem *> items() const ;
private:
    QList<QGraphicsItem * > m_items;
};

} // spacename gvf
