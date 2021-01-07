#include "GraphicsPathItem.h"
#include <QPainterPath>

namespace gvf {

GraphicsPathItem::GraphicsPathItem(GraphicsItem *parent)
    : GraphicsItem(parent) {
    m_painterPath = new QPainterPath;
}

void GraphicsPathItem::setBeginPoint(const QPointF &point)
{
    m_beginPoint = mapFromScene(point);
    m_painterPath->moveTo(m_beginPoint);
    m_boundingRect = m_painterPath->controlPointRect();
}

void GraphicsPathItem::setEndPoint(const QPointF &point)
{
    m_endPoint = mapFromScene(point);
    m_painterPath->lineTo(m_endPoint);
    m_boundingRect = m_painterPath->controlPointRect();
    update();
}

QRectF GraphicsPathItem::boundingRect() const
{
    qDebug() << m_boundingRect;
    return m_boundingRect;
}

QPainterPath GraphicsPathItem::shape() const
{
    QPainterPath path;
    path.addRect(m_boundingRect);
    return qt_graphicsItem_shapeFromPath(path,pen());
}

QGraphicsItem *GraphicsPathItem::duplicate() const
{
    return nullptr;
}

void GraphicsPathItem::control(int dir, const QPointF &delta)
{

}

void GraphicsPathItem::updateCoordinate()
{

}

void GraphicsPathItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPen pen = painter->pen();
    pen.setCosmetic(true);
    pen.setWidth(3);
    pen.setBrush(m_pen.color());
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    painter->setPen(pen);
    //m_painterPath->lineTo(m_endPoint);

    painter->setRenderHint(QPainter::Antialiasing);
    painter->drawPath(*m_painterPath);
}

} // namespace gvf
