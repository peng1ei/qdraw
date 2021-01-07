#include "GraphicsPathItem.h"
#include "DrawTool/DrawTool.h"
#include <QPainterPath>

namespace gvf {

GraphicsPathItem::GraphicsPathItem(GraphicsItem *parent)
    : GraphicsItem(parent) {
    m_painterPath = new QPainterPath;
    m_eraserPainterPath = new QPainterPath;
    m_penWidth = 3;
    m_eraserWidth = m_penWidth;
    m_isEraser = false;
}

void GraphicsPathItem::setBeginPoint(const QPointF &point)
{
    if (!m_isEraser) {
        prepareGeometryChange();
        m_beginPoint = mapFromScene(point);
        m_painterPath->moveTo(m_beginPoint);
        m_boundingRect = m_painterPath->controlPointRect();
        updatehandles();
        update();
    } else {
        //prepareGeometryChange();
        //m_beginPoint = mapFromScene(point);
        m_eraserPainterPath->moveTo(mapFromScene(point));
        //m_boundingRect = m_painterPath->controlPointRect();
        //updatehandles();
        update();
    }
}

void GraphicsPathItem::setEndPoint(const QPointF &point)
{
    if (!m_isEraser) {
        prepareGeometryChange();
        m_endPoint = mapFromScene(point);
        m_painterPath->lineTo(m_endPoint);
        m_boundingRect = m_painterPath->controlPointRect();
        updatehandles();
        update();
    } else {
        m_eraserPainterPath->lineTo(mapFromScene(point));
        update();
    }
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
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);

    //if (!m_isEraser) {
        pen.setWidth(m_penWidth);
        pen.setBrush(m_pen.color());
    //} else {
        //pen.setWidth(m_eraserWidth);
        //pen.setBrush(Qt::red);
    //}

    painter->setPen(pen);
    painter->setRenderHint(QPainter::Antialiasing);

    //painter->setCompositionMode(QPainter::CompositionMode_DestinationIn);
    painter->drawPath(*m_painterPath);

    // TODO 用背景图片进行绘制
    // TODO 分图层实现绘制
    if (m_isEraser) {
        m_eraserWidth = 10;
        pen.setWidth(m_eraserWidth);
        pen.setBrush(QColor(255,0,0, 10));
        painter->setPen(pen);
        //painter->setCompositionMode(QPainter::CompositionMode_DestinationAtop);
        painter->drawPath(*m_eraserPainterPath);
        //painter->eraseRect(m_eraserPainterPath->boundingRect());
    }

    if (option->state & QStyle::State_Selected)
        qt_graphicsItem_highlightSelected(this, painter, option);
}

} // namespace gvf
