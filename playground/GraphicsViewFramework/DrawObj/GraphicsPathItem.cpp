#include "GraphicsPathItem.h"
#include "DrawTool/DrawTool.h"
#include <QPainterPath>

namespace gvf {

GraphicsPathItem::GraphicsPathItem(GraphicsItem *parent)
    : GraphicsItem(parent) {
    m_painterPath = new QPainterPath;
    m_penWidth = 3;
}

void GraphicsPathItem::setBeginPoint(const QPointF &point)
{
    prepareGeometryChange();
    m_beginPoint = mapFromScene(point);
    m_painterPath->moveTo(m_beginPoint);
    m_boundingRect = m_painterPath->controlPointRect();
    updatehandles();
    update();
}

void GraphicsPathItem::setEndPoint(const QPointF &point)
{
    prepareGeometryChange();
    m_endPoint = mapFromScene(point);
    m_painterPath->lineTo(m_endPoint);
    m_boundingRect = m_painterPath->controlPointRect();
    updatehandles();
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

void GraphicsPathItem::updatehandles()
{
    GraphicsItem::updatehandles();
}

void GraphicsPathItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPen pen = painter->pen();
    pen.setCosmetic(true);
    pen.setWidth(m_penWidth);
    pen.setBrush(m_pen.color());
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    painter->setPen(pen);

    painter->setRenderHint(QPainter::Antialiasing);
    painter->drawPath(*m_painterPath);

    if (option->state & QStyle::State_Selected)
        qt_graphicsItem_highlightSelected(this, painter, option);
}

} // namespace gvf
