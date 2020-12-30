#include "GraphicsEllipseItem.h"

namespace gvf {

GraphicsEllipseItem::GraphicsEllipseItem(const QRect & rect ,QGraphicsItem *parent)
    :GraphicsRectItem(rect,parent)
{
    qDebug() << "m_handles size before: " << m_handles.size();
    for (int i = 0; i < 4; i++) {
        m_handles.pop_back();
    }

    qDebug() << "m_handles size after: " << m_handles.size();

    m_startAngle = 40;
    m_spanAngle  = 400;

    // TODO 与Rect重复添加
    SizeHandleRect *shr = new SizeHandleRect(this, Handle_Shape+1 , true);
    shr->setHandleType(SelectionHandleType::ShapeHandle);
    m_handles.push_back(shr);
    shr = new SizeHandleRect(this, Handle_Shape+2 , true);
    shr->setHandleType(SelectionHandleType::ShapeHandle);
    m_handles.push_back(shr);
    shr = new SizeHandleRect(this, Handle_Shape+3 , true);
    shr->setHandleType(SelectionHandleType::ShapeHandle);
    m_handles.push_back(shr);
    updatehandles();
}

QPainterPath GraphicsEllipseItem::shape() const
{
    QPainterPath path;
    int startAngle = m_startAngle <= m_spanAngle ? m_startAngle : m_spanAngle;
    int endAngle = m_startAngle >= m_spanAngle ? m_startAngle : m_spanAngle;
    if(endAngle - startAngle > 360)
        endAngle = startAngle + 360;

    if (m_localRect.isNull())
        return path;
    if ((endAngle - startAngle) % 360 != 0 ) {
        path.moveTo(m_localRect.center());
        path.arcTo(m_localRect, startAngle, endAngle - startAngle);
    } else {
        path.addEllipse(m_localRect);
    }
    path.closeSubpath();
    return path;
}

void GraphicsEllipseItem::control(int dir, const QPointF & delta)
{
    QPointF local = mapFromScene(delta);

    switch (dir) {
    case Handle_Shape + 1:
    {
        qreal len_y = local.y() - m_localRect.center().y();
        qreal len_x = local.x() - m_localRect.center().x();
        m_startAngle = -atan2(len_y,len_x)*180/M_PI;
    }
        break;
    case Handle_Shape + 2:
    {
        qreal len_y = local.y() - m_localRect.center().y();
        qreal len_x = local.x() - m_localRect.center().x();
        m_spanAngle = -atan2(len_y,len_x)*180/M_PI;
        break;
    }
   default:
        break;
    }

    prepareGeometryChange();
    if ( m_startAngle > m_spanAngle )
        m_startAngle-=360;
    if ( m_spanAngle < m_startAngle ){
        qreal tmp = m_spanAngle;
        m_spanAngle = m_startAngle;
        m_startAngle = tmp;
    }

    if ( qAbs(m_spanAngle-m_startAngle) > 360 ){
        m_startAngle = 40;
        m_spanAngle = 400;
    }
    updatehandles();

    switch (dir) {
    case LeftTop:
        {
            QRectF oldRect = m_localRect;
            prepareGeometryChange();
            // TODO 修改Item在Scene的坐标不知道会不会带来其它问题
            setPos(delta);
            m_localRect = QRectF(0,0,oldRect.width()-local.x(),oldRect.height()-local.y());
            m_width = m_localRect.width();
            m_height = m_localRect.height();
            updatehandles();
        }
        break;
    case RightTop:
        {
            QRectF oldRect = m_localRect;
            prepareGeometryChange();
            // TODO 修改Item在Scene的坐标不知道会不会带来其它问题
            //setPos(delta);
            //QPointF p = pos();
            //p.setY(delta.y());
            //setPos(p);
            setY(delta.y());
            m_localRect = QRectF(0,0,local.x(),oldRect.height() - local.y());
            m_width = m_localRect.width();
            m_height = m_localRect.height();
            updatehandles();
        }
        break;
    case RightBottom:
        {
            prepareGeometryChange();
            m_localRect = QRectF(0,0, local.x(), local.y());
            m_width = m_localRect.width();
            m_height = m_localRect.height();
            updatehandles();
        }
        break;
    case LeftBottom:
        {
            QRectF oldRect = m_localRect;
            prepareGeometryChange();
            // TODO 修改Item在Scene的坐标不知道会不会带来其它问题
            setX(delta.x());
            m_localRect = QRectF(0,0,oldRect.width() - local.x(),local.y());
            m_width = m_localRect.width();
            m_height = m_localRect.height();
            updatehandles();
        }
        break;
    case Top: // Top
        {
            QRectF oldRect = m_localRect;
            prepareGeometryChange();
            // TODO 修改Item在Scene的坐标不知道会不会带来其它问题
            setY(delta.y());
            m_localRect = QRectF(0,0,oldRect.width(),oldRect.height() - local.y());
            m_width = m_localRect.width();
            m_height = m_localRect.height();
            updatehandles();
        }
        break;
    case Bottom: // Bottom
        {
            QRectF oldRect = m_localRect;
            prepareGeometryChange();
            m_localRect = QRectF(0,0,oldRect.width(),local.y());
            m_width = m_localRect.width();
            m_height = m_localRect.height();
            updatehandles();
        }
        break;
    case Right: // Right
        {
            QRectF oldRect = m_localRect;
            prepareGeometryChange();
            // TODO 修改Item在Scene的坐标不知道会不会带来其它问题
            m_localRect = QRectF(0,0,local.x(),oldRect.height());
            m_width = m_localRect.width();
            m_height = m_localRect.height();
            updatehandles();
        }
        break;
    case Left: // Left
        {
            QRectF oldRect = m_localRect;
            prepareGeometryChange();
            setX(delta.x());
            m_localRect = QRectF(0,0, oldRect.width() - local.x(),oldRect.height());
            m_width = m_localRect.width();
            m_height = m_localRect.height();
            updatehandles();
        }
        break;
    default:
        break;
    }
}

QRectF GraphicsEllipseItem::boundingRect() const
{
    return shape().controlPointRect();
}

QGraphicsItem *GraphicsEllipseItem::duplicate() const
{
    GraphicsEllipseItem * item = new GraphicsEllipseItem( m_localRect.toRect() );
    item->m_width = width();
    item->m_height = height();
    item->m_startAngle = m_startAngle;
    item->m_spanAngle   = m_spanAngle;

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

bool GraphicsEllipseItem::loadFromXml(QXmlStreamReader *xml)
{
    m_startAngle = xml->attributes().value("startAngle").toInt();
    m_spanAngle  = xml->attributes().value("spanAngle").toInt();
    readBaseAttributes(xml);
    xml->skipCurrentElement();
    updateCoordinate();
    return true;
}

bool GraphicsEllipseItem::saveToXml(QXmlStreamWriter * xml)
{
    xml->writeStartElement(tr("ellipse"));
    xml->writeAttribute("startAngle",QString("%1").arg(m_startAngle));
    xml->writeAttribute("spanAngle",QString("%1").arg(m_spanAngle));

    writeBaseAttributes(xml);
    xml->writeEndElement();
    return true;
}


void GraphicsEllipseItem::updatehandles()
{
    GraphicsItem::updatehandles();
    QRectF local = QRectF(-m_width/2,-m_height/2,m_width,m_height);
    QPointF delta = local.center() - m_localRect.center();

    qreal x = (m_width/2) * cos( -m_startAngle * M_PI / 180 );
    qreal y = (m_height/2) * sin( -m_startAngle * M_PI / 180);

    m_handles.at(Handle_Shape + 0)->move(x-delta.x(),y-delta.y());
    x = (m_width/2) * cos( -m_spanAngle * M_PI / 180);
    y = (m_height/2) * sin(-m_spanAngle * M_PI / 180);
    m_handles.at(Handle_Shape + 1)->move(x-delta.x(),y-delta.y());

    m_handles.at(Handle_Shape + 2)->move(m_localRect.center().x(), m_localRect.center().y());

    m_handles.at(Handle_Shape + 0)->setBrushColor(m_brush.color());
    m_handles.at(Handle_Shape + 1)->setBrushColor(m_brush.color());
    m_handles.at(Handle_Shape + 2)->setBrushColor(m_brush.color());
}

void GraphicsEllipseItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    QColor c = brushColor();
    QRectF rc = m_localRect;

    qreal radius = qMax(rc.width(),rc.height());

/*
    QConicalGradient  result(rc.center(),-45);

    QColor niceBlue(150, 150, 200);
    result.setColorAt(0.0, c.dark(200));
    result.setColorAt(0.2, niceBlue);
    result.setColorAt(0.5, c.light(120));
    result.setColorAt(1.0, c.dark(200));
*/
    //painter->setPen(pen());
    //QBrush b(c);
    //painter->setBrush(b);

    QPen tpen = painter->pen();
    tpen.setWidthF(1);
    tpen.setColor(m_pen.color());
    tpen.setCosmetic(true);
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setRenderHint(QPainter::SmoothPixmapTransform);
    painter->setPen(tpen);

    widget->setAttribute(Qt::WA_TranslucentBackground, true);
    painter->setBrush(QColor(m_brush.color().red(),
                            m_brush.color().green(),
                            m_brush.color().blue(),m_alpha));//最后一位是设置透明属性（在0-255取值）

    int startAngle = m_startAngle <= m_spanAngle ? m_startAngle : m_spanAngle;
    int endAngle = m_startAngle >= m_spanAngle ? m_startAngle : m_spanAngle;
    if(endAngle - startAngle > 360)
        endAngle = startAngle + 360;

    if (qAbs(endAngle-startAngle) % (360) == 0)
        painter->drawEllipse(m_localRect);
    else
        painter->drawPie(m_localRect, startAngle * 16 , (endAngle-startAngle) * 16);


    ///return;
    if (option->state & QStyle::State_Selected)
        qt_graphicsItem_highlightSelected(this, painter, option);
}

}// namespace gvf
