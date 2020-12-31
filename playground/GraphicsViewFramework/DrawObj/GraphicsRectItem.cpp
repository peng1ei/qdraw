#include "GraphicsRectItem.h"
#include <QPainter>
#include <QWidget>
#include <QStyleOptionGraphicsItem>

namespace gvf {

GraphicsRectItem::GraphicsRectItem(const QRectF & rect , bool isRound , QGraphicsItem *parent)
    :GraphicsItem(parent)
    ,m_isRound(isRound)
    ,m_fRatioY(1/3.0)
    ,m_fRatioX(1/10.0)
{
    m_width = rect.width();
    m_height = rect.height();
    m_initialRect = rect;
    m_localRect = m_initialRect;
    m_localRect = rect;
    m_originPoint = QPointF(0,0);
    if( m_isRound ){
        // 圆角矩形
        SizeHandleRect *shr = new SizeHandleRect(this, Handle_Shape + 1 , true);
        shr->setHandleType(SelectionHandleType::ShapeHandle);
        m_handles.push_back(shr);
        shr = new SizeHandleRect(this, Handle_Shape + 2 , true);
        shr->setHandleType(SelectionHandleType::ShapeHandle);
        m_handles.push_back(shr);

        //shr = new SizeHandleRect(this, 11 , true);
        //m_handles.push_back(shr);
    } else {
        // 直角矩形
        for (int i = 1; i <= 4; i++) {
            SizeHandleRect *shr = new SizeHandleRect(this, Handle_Shape + i, true);
            shr->setHandleType(SelectionHandleType::ShapeHandle);
            m_handles.push_back(shr);
            //m_handles.push_back(shr);
        }
    }

    updatehandles();
}

QRectF GraphicsRectItem::boundingRect() const
{
    return m_localRect;
}

QPainterPath GraphicsRectItem::shape() const
{
    QPainterPath path;
    double rx,ry;
    if(m_fRatioX<=0)
       rx=0;
    else {
        rx = m_width * m_fRatioX + 0.5;
    }
    if ( m_fRatioY <=0 )
        ry = 0;
    else
        ry = m_height * m_fRatioY + 0.5;

    if ( m_isRound )
        path.addRoundedRect(rect(),rx,ry);
    else
        path.addRect(rect());
    return path;
}

void GraphicsRectItem::control(int dir, const QPointF & delta)
{
    qDebug() << "delta: " << delta;
    QPointF local = mapFromParent(delta);
    qDebug() << "local: " << local;
    qDebug() << "local scene: " << mapFromScene(delta);

    if (m_isRound) {
        switch (dir) {
        case Handle_Shape + 1:
        {
            QRectF delta1 = rect();
            int y = local.y();
            if(y> delta1.center().y() )
                y = delta1.center().y();
            if(y<delta1.top())
                y=delta1.top();
            int H= delta1.height();
            if(H==0)
                H=1;
            m_fRatioY = std::abs(((float)(delta1.top()-y)))/H;

            prepareGeometryChange();
            updatehandles();
        }
            break;
        case Handle_Shape + 2:
        {
            QRectF delta1 = rect();
            int x = local.x();
            if(x < delta1.center().x() )
                x = delta1.center().x();
            if(x>delta1.right())
                x=delta1.right();
            int W= delta1.width();
            if(W==0)
                W=1;
            m_fRatioX = std::abs(((float)(delta1.right()-x)))/W;

            prepareGeometryChange();
            updatehandles();
            break;
        }
        case Handle_Shape + 3:
        {
            // TODO
            //setTransform(transform().translate(-local.x(),-local.y()));
            //setTransformOriginPoint(local.x(),local.y());
            //setTransform(transform().translate(local.x(),local.y()));
            m_originPoint = local;
        }
            break;
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
        case LeftBottom:// left-bottom
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
        return;
    }

    switch (dir) {
    case LeftTop:
    case Handle_Shape+1:// left-top
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
    case Handle_Shape+2: // right-top
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
    case Handle_Shape+3: // right-bottom
        {
            prepareGeometryChange();
            m_localRect = QRectF(0,0, local.x(), local.y());
            m_width = m_localRect.width();
            m_height = m_localRect.height();
            updatehandles();
        }
        break;
    case LeftBottom:
    case Handle_Shape+4: // left-bottom
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

    qDebug() << "m_localRect: " << m_localRect;
}

void GraphicsRectItem::stretch(int handle , double sx, double sy, const QPointF & origin)
{
    Q_UNUSED(handle)
    QTransform trans  ;

#if 0
    switch (handle) {
    case Right:
    case Left:
        sy = 1;
        break;
    case Top:
    case Bottom:
        sx = 1;
        break;
    default:
        break;
    }
#endif

    qDebug() << "sx: " << sx << " sy: " << sy;
    qDebug() << "origin: " << origin;

    //opposite_ = origin;

    trans.translate(origin.x(),origin.y());
    trans.scale(sx,sy);
    trans.translate(-origin.x(),-origin.y());

    prepareGeometryChange();

    qDebug() << "m_initialRect: " << m_initialRect;

    // [by pl]
    //m_localRect = trans.mapRect(m_initialRect);
    m_localRect = trans.mapRect(QRectF(0,0,1,1));
    m_width = m_localRect.width();
    m_height = m_localRect.height();
    updatehandles();
}

void GraphicsRectItem::updateCoordinate()
{

    QPointF pt1,pt2,delta;

    pt1 = mapToScene(transformOriginPoint());
    pt2 = mapToScene(m_localRect.center());
    delta = pt1 - pt2;

    //qDebug() << "m_localRect: " << m_localRect;
    if (!parentItem() ){
        prepareGeometryChange();
        //[by pl]
        //m_localRect = QRectF(-m_width/2,-m_height/2,m_width,m_height);
        m_localRect = QRectF(0,0,m_width,m_height);
        m_width = m_localRect.width();
        m_height = m_localRect.height();
        setTransform(transform().translate(delta.x(),delta.y()));
        setTransformOriginPoint(m_localRect.center());
        //moveBy(-delta.x(),-delta.y());
        setTransform(transform().translate(-delta.x(),-delta.y()));
        //opposite_ = QPointF(0,0);
        updatehandles();
    }

    //qDebug() << "m_localRect 2: " << m_localRect;

    m_initialRect = m_localRect;
}

void GraphicsRectItem::move(const QPointF &point)
{
    moveBy(point.x(),point.y());
}

QGraphicsItem *GraphicsRectItem::duplicate() const
{
    GraphicsRectItem * item = new GraphicsRectItem( rect().toRect(),m_isRound);
    item->m_width = width();
    item->m_height = height();
    item->setPos(pos().x(),pos().y());
    item->setPen(pen());
    item->setBrush(brush());
    item->setTransform(transform());
    item->setTransformOriginPoint(transformOriginPoint());
    item->setRotation(rotation());
    item->setScale(scale());
    item->setZValue(zValue()+0.1);
    item->m_fRatioY = m_fRatioY;
    item->m_fRatioX = m_fRatioX;
    item->updateCoordinate();
    return item;
}

bool GraphicsRectItem::loadFromXml(QXmlStreamReader * xml )
{
    m_isRound = (xml->name() == tr("roundrect"));
    if ( m_isRound ){
        m_fRatioX = xml->attributes().value(tr("rx")).toDouble();
        m_fRatioY = xml->attributes().value(tr("ry")).toDouble();
    }
    readBaseAttributes(xml);
    updateCoordinate();
    xml->skipCurrentElement();
    return true;
}

bool GraphicsRectItem::saveToXml(QXmlStreamWriter * xml)
{
    if ( m_isRound ){
        xml->writeStartElement(tr("roundrect"));
        xml->writeAttribute(tr("rx"),QString("%1").arg(m_fRatioX));
        xml->writeAttribute(tr("ry"),QString("%1").arg(m_fRatioY));
    }
    else
        xml->writeStartElement(tr("rect"));

    writeBaseAttributes(xml);
    xml->writeEndElement();
    return true;
}

void GraphicsRectItem::hoverEnterEvent(QGraphicsSceneHoverEvent *e)
{

    m_alpha = 100;
    update();
    //QGraphicsItem::hoverEnterEvent(e);
    //setCursor(Qt::OpenHandCursor);
}

void GraphicsRectItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *e)
{
    m_alpha = 50;
    update();
    //QGraphicsItem::hoverLeaveEvent(e);
    //setCursor(Qt::ArrowCursor);
}

void GraphicsRectItem::updatehandles()
{
    const QRectF &geom = this->boundingRect();
    GraphicsItem::updatehandles();
    if ( m_isRound ){
        qreal y = geom.top() + geom.height() * m_fRatioY + 0.5;
        if (y > geom.top() + m_height/2)
            y = geom.top() + m_height/2;

        qreal x = geom.right() - geom.width() * m_fRatioX - 0.5;
        if (x < geom.right() - m_width / 2)
            x = geom.right() - m_width / 2;

        m_handles[Handle_Shape+0]->move( geom.right() , y);
        m_handles[Handle_Shape+1]->move( x, geom.top());
        //m_handles[Handle_Shape+2]->move(m_originPoint.x(),m_originPoint.y());

        m_handles[Handle_Shape+0]->setBrushColor(m_brush.color());
        m_handles[Handle_Shape+1]->setBrushColor(m_brush.color());
    } else {
#if 1
        m_handles[Handle_Shape+0]->move(geom.left(), geom.top());
        m_handles[Handle_Shape+1]->move(geom.right(), geom.top());
        m_handles[Handle_Shape+2]->move(geom.right(), geom.bottom());
        m_handles[Handle_Shape+3]->move(geom.left(), geom.bottom());

        m_handles[Handle_Shape+0]->setBrushColor(m_brush.color());
        m_handles[Handle_Shape+1]->setBrushColor(m_brush.color());
        m_handles[Handle_Shape+2]->setBrushColor(m_brush.color());
        m_handles[Handle_Shape+3]->setBrushColor(m_brush.color());
#endif
    }
}

void GraphicsRectItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
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

   double rx,ry;
   if(m_fRatioX<=0)
      rx=0;
   else {
       rx = m_width * m_fRatioX + 0.5;
   }
   if ( m_fRatioY <=0 )
       ry = 0;
   else
       ry = m_height * m_fRatioY + 0.5;

   if ( m_isRound )
       painter->drawRoundedRect(rect(),rx,ry);
   else
       painter->drawRect(m_localRect);

#if 0 // 绘制十字丝标注
   painter->setPen(Qt::blue);
   painter->drawLine(QLine(QPoint(opposite_.x()-6,opposite_.y()),QPoint(opposite_.x()+6,opposite_.y())));
   painter->drawLine(QLine(QPoint(opposite_.x(),opposite_.y()-6),QPoint(opposite_.x(),opposite_.y()+6)));
#endif

   //return;
   if (option->state & QStyle::State_Selected)
       qt_graphicsItem_highlightSelected(this, painter, option);
/*

   QPolygonF pts;
   pts<<m_localRect.topLeft()<<m_localRect.topRight()<<m_localRect.bottomRight()<<m_localRect.bottomLeft();
   pts = mapToScene(pts);
   QRectF bound = RecalcBounds(pts);


   qDebug()<<m_localRect<<bound;
    pts.clear();
   pts<<bound.topLeft()<<bound.topRight()<<bound.bottomRight()<<bound.bottomLeft();
   pts = mapFromScene(pts);
   if ( scene() ){
   painter->save();
   painter->setPen(Qt::blue);
   painter->setBrush(Qt::NoBrush);
   painter->drawPolygon(pts);
   painter->restore();
   }
*/
}

}// namespace gvf
