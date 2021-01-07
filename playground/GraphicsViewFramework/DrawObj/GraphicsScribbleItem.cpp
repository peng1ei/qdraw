#include "GraphicsScribbleItem.h"
#include <QLine>

namespace gvf {

gvf::GraphicsScribbleItem::GraphicsScribbleItem(qreal w, qreal h, QGraphicsItem *parent)
    : GraphicsItem(parent),
    m_canvasWidth(w),
    m_canvasHeight(h) {
    m_beginPoint = QPointF();
    m_endPoint = QPointF();
    m_minX = 0;
    m_minY = 0;
    m_maxX = 0;
    m_maxY = 0;
    QImage pixmap(m_canvasWidth, m_canvasHeight,QImage::Format_RGBA8888);
    pixmap.fill(Qt::transparent);
    out = pixmap;
}

QRectF gvf::GraphicsScribbleItem::boundingRect() const
{
    return QRectF(0, 0, m_canvasWidth, m_canvasHeight);
}

QPainterPath gvf::GraphicsScribbleItem::shape() const
{
    return QPainterPath();
}

QGraphicsItem *gvf::GraphicsScribbleItem::duplicate() const
{
    return nullptr;
}

void GraphicsScribbleItem::control(int dir, const QPointF &delta)
{
    m_endPoint = delta;
}

void GraphicsScribbleItem::updateCoordinate()
{

}

void gvf::GraphicsScribbleItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPainter p(&out);
    pen.setWidth(3);
    pen.setBrush(m_pen.color());
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    p.setPen(pen);
    p.setRenderHint(QPainter::Antialiasing);
    //p.setRenderHint(QPainter::SmoothPixmapTransform);
    p.setPen(pen);

    if (m_beginPoint.isNull())
        m_beginPoint = m_endPoint;

    p.drawLine(m_beginPoint, m_endPoint);
    m_beginPoint = m_endPoint;

    painter->setRenderHint(QPainter::Antialiasing);
    //painter->setRenderHint(QPainter::SmoothPixmapTransform);
    painter->drawImage(0,0,out);
}

}
