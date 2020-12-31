#include "GraphicsBezier.h"

namespace gvf {

GraphicsBezier::GraphicsBezier(bool bbezier,QGraphicsItem *parent)
    :GraphicsPolygonItem(parent)
    ,m_isBezier(bbezier)
{
    //m_brush = QBrush(Qt::NoBrush);
}

QPainterPath GraphicsBezier::shape() const
{
    QPainterPath path;
    path.moveTo(m_points.at(0));
    int i=1;
    while (m_isBezier && ( i + 2 < m_points.size())) {
        path.cubicTo(m_points.at(i), m_points.at(i+1), m_points.at(i+2));
        i += 3;
    }
    while (i < m_points.size()) {
        path.lineTo(m_points.at(i));
        ++i;
    }

    return qt_graphicsItem_shapeFromPath(path,pen());
}

QGraphicsItem *GraphicsBezier::duplicate() const
{
    GraphicsBezier * item = new GraphicsBezier( );
    item->m_width = width();
    item->m_height = height();
    item->m_points = m_points;
    item->m_isBezier = m_isBezier;
    for ( int i = 0 ; i < m_points.size() ; ++i ){
        auto sizehandle = new SizeHandleRect(item,Left+i+1,true);
        sizehandle->setBrushColor(m_brush.color());
        item->m_handles.push_back(sizehandle);
    }
    item->setPos(pos().x(),pos().y());
    item->setPen(pen());
    item->setBrush(brush());
    item->setTransform(transform());
    item->setTransformOriginPoint(transformOriginPoint());
    item->setRotation(rotation());
    item->setScale(scale());
    item->setZValue(zValue()+0.1);
    item->updateCoordinate();
    return item;
}

bool GraphicsBezier::loadFromXml(QXmlStreamReader *xml)
{
    m_isBezier = (xml->name() == tr("bezier"));
    return GraphicsPolygonItem::loadFromXml(xml);
}

bool GraphicsBezier::saveToXml(QXmlStreamWriter *xml)
{
    if ( m_isBezier )
        xml->writeStartElement("bezier");
    else
        xml->writeStartElement("polyline");

    writeBaseAttributes(xml);

    for ( int i = 0 ; i < m_points.count();++i){
        xml->writeStartElement("point");
        xml->writeAttribute("x",QString("%1").arg(m_points[i].x()));
        xml->writeAttribute("y",QString("%1").arg(m_points[i].y()));
        xml->writeEndElement();
    }
    xml->writeEndElement();

    return true;
}

// 绘制多折线，贝塞尔曲线
void GraphicsBezier::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QPainterPath path;
//    painter->setPen(pen());
//    painter->setBrush(brush());
    QPen tpen = painter->pen();
    tpen.setWidthF(1);
    tpen.setColor(m_pen.color());
    tpen.setCosmetic(true); // 设置外边框粗细不变
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setRenderHint(QPainter::SmoothPixmapTransform);
    painter->setPen(tpen);
    painter->setBrush(Qt::NoBrush);

    path.moveTo(m_points.at(0));

    int i=1;
    while (m_isBezier && ( i + 2 < m_points.size())) {
        // 绘制贝塞尔曲线
        path.cubicTo(m_points.at(i), m_points.at(i+1), m_points.at(i+2));
        i += 3;
    }
    while (i < m_points.size()) {
        path.lineTo(m_points.at(i));
        ++i;
    }
    painter->drawPath(path);

    // 暂时不知道是干嘛的
    if (option->state & QStyle::State_Selected){
       //painter->setPen(QPen(Qt::lightGray, 0, Qt::SolidLine));
       painter->setBrush(Qt::NoBrush);
       painter->drawPolyline(m_points);
    }

   ///return;
   // 绘制外接矩形，个人认为可以不必画出
   if (option->state & QStyle::State_Selected)
       qt_graphicsItem_highlightSelected(this, painter, option);
}

}// namespace gvf
