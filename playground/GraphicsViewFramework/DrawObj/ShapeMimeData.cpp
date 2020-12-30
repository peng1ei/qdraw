#include "ShapeMimeData.h"
#include "AbstractShapeType.hpp"
#include <QGraphicsItem>

namespace gvf {

ShapeMimeData::ShapeMimeData(QList<QGraphicsItem *> items)
{
    foreach (QGraphicsItem *item , items ) {
       AbstractShape *sp = qgraphicsitem_cast<AbstractShape*>(item);
       m_items.append(sp->duplicate());
    }
}
ShapeMimeData::~ShapeMimeData()
{
    foreach (QGraphicsItem *item , m_items ) {
        delete item;
    }
    m_items.clear();
}

QList<QGraphicsItem *> ShapeMimeData::items() const
{
    return m_items;
}

}// namespace gvf
