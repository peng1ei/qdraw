#include "GraphicsItem.h"
#include <QPainter>
#include <QStyleOptionGraphicsItem>

namespace gvf {

GraphicsItem::GraphicsItem(QGraphicsItem *parent)
    :AbstractShapeType<QGraphicsItem>(parent)
{
    /*
    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect;
    effect->setBlurRadius(4);
    setGraphicsEffect(effect);
   */

    // 预定义外界矩形的角点，比如当选择某个item的时候，可以显示这些角点
    // handles
    m_handles.reserve(Left);
    for (int i = LeftTop; i <= Left; ++i) {
        SizeHandleRect *shr = new SizeHandleRect(this,i,true);
        m_handles.push_back(shr);
    }

    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    this->setAcceptHoverEvents(true);
}

QPixmap GraphicsItem::image() {
    QPixmap pixmap(64, 64);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    setPen(QPen(Qt::black));
    setBrush(Qt::white);
    QStyleOptionGraphicsItem *styleOption = new QStyleOptionGraphicsItem;
//    painter.translate(m_localRect.center().x(),m_localRect.center().y());
    paint(&painter,styleOption);
    delete styleOption;
    return pixmap;
}


// 控制绘制时外接矩形的角点和四边的中点
void GraphicsItem::updatehandles()
{
    const QRectF &geom = this->boundingRect();

    const Handles::iterator hend =  m_handles.end();
    for (Handles::iterator it = m_handles.begin(); it != hend; ++it) {
        SizeHandleRect *hndl = *it;
        switch (hndl->dir()) {
        case LeftTop:
            hndl->move(geom.x() , geom.y() );
            break;
        case Top:
            hndl->move(geom.x() + geom.width() / 2 , geom.y() );
            break;
        case RightTop:
            hndl->move(geom.x() + geom.width() , geom.y() );
            break;
        case Right:
            hndl->move(geom.x() + geom.width() , geom.y() + geom.height() / 2 );
            break;
        case RightBottom:
            hndl->move(geom.x() + geom.width() , geom.y() + geom.height() );
            break;
        case Bottom:
            hndl->move(geom.x() + geom.width() / 2 , geom.y() + geom.height() );
            break;
        case LeftBottom:
            hndl->move(geom.x(), geom.y() + geom.height());
            break;
        case Left:
            hndl->move(geom.x(), geom.y() + geom.height() / 2);
            break;
        default:
            break;
        }
    }
}

void GraphicsItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    Q_UNUSED(event);
}

bool GraphicsItem::readBaseAttributes(QXmlStreamReader *xml)
{
    qreal x = xml->attributes().value(tr("x")).toDouble();
    qreal y = xml->attributes().value(tr("y")).toDouble();
    m_width = xml->attributes().value("width").toDouble();
    m_height = xml->attributes().value("height").toDouble();
    setZValue(xml->attributes().value("z").toDouble());
    setRotation(xml->attributes().value("rotate").toDouble());
    setPos(x,y);
    return true;
}

bool GraphicsItem::writeBaseAttributes(QXmlStreamWriter *xml)
{
    xml->writeAttribute(tr("rotate"),QString("%1").arg(rotation()));
    xml->writeAttribute(tr("x"),QString("%1").arg(pos().x()));
    xml->writeAttribute(tr("y"),QString("%1").arg(pos().y()));
    xml->writeAttribute(tr("z"),QString("%1").arg(zValue()));
    xml->writeAttribute(tr("width"),QString("%1").arg(m_width));
    xml->writeAttribute(tr("height"),QString("%1").arg(m_height));
    return true;
}

// item 选中状态改变
QVariant GraphicsItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    // TODO 添加DrawTool的状态判断
    if ( change == QGraphicsItem::ItemSelectedHasChanged) {
        QGraphicsItemGroup *g = dynamic_cast<QGraphicsItemGroup*>(parentItem());
        if (!g)
            // [2020-12-26 23:25 by pl] 无论Item是否被选中，都显示角点标注
            //setState(value.toBool() ? SelectionHandleActive : SelectionHandleActive);
            setState(value.toBool() ? SelectionHandleActive : SelectionHandleOff);
        else{
            setSelected(false);
            return QVariant::fromValue<bool>(false);
        }
    }
    /*
    else if (change == ItemPositionChange && scene()) {
        // value is the new position.
        QPointF newPos = value.toPointF();
        QRectF rect = scene()->sceneRect();
        if (!rect.contains(newPos)) {
            // Keep the item inside the scene rect.
            newPos.setX(qMin(rect.right()-boundingRect().width()/2, qMax(newPos.x(), rect.left()+boundingRect().width()/2)));
            newPos.setY(qMin(rect.bottom()-boundingRect().height()/2, qMax(newPos.y(), rect.top()+boundingRect().height()/2)));
            return newPos;
        }
    }
    */
    return QGraphicsItem::itemChange(change, value);
}

//-----------------------------------------------------
QRectF RecalcBounds(const QPolygonF&  pts)
{
    QRectF bounds(pts[0], QSize(0, 0));
    for (int i = 1; i < pts.count(); ++i)
    {
        if (pts[i].x() < bounds.left())
            bounds.setLeft(pts[i].x());
        if (pts[i].x() > bounds.right())
            bounds.setRight(pts[i].x());
        if (pts[i].y() < bounds.top())
            bounds.setTop(pts[i].y());
        if (pts[i].y() > bounds.bottom())
            bounds.setBottom (pts[i].y());
    }
    bounds = bounds.normalized();
    return bounds;
}

QPainterPath qt_graphicsItem_shapeFromPath(const QPainterPath &path, const QPen &pen)
{
    // We unfortunately need this hack as QPainterPathStroker will set a width of 1.0
    // if we pass a value of 0.0 to QPainterPathStroker::setWidth()
    const qreal penWidthZero = qreal(0.00000001);

    if (path == QPainterPath() || pen == Qt::NoPen)
        return path;
    QPainterPathStroker ps;
    ps.setCapStyle(pen.capStyle());
    if (pen.widthF() <= 0.0)
        ps.setWidth(penWidthZero);
    else
        ps.setWidth(pen.widthF());
    ps.setJoinStyle(pen.joinStyle());
    ps.setMiterLimit(pen.miterLimit());
    QPainterPath p = ps.createStroke(path);
    p.addPath(path);
    return p;
}

void qt_graphicsItem_highlightSelected(
    QGraphicsItem *item, QPainter *painter, const QStyleOptionGraphicsItem *option)
{
    const QRectF murect = painter->transform().mapRect(QRectF(0, 0, 1, 1));
    if (qFuzzyIsNull(qMax(murect.width(), murect.height())))
        return;

    const QRectF mbrect = painter->transform().mapRect(item->boundingRect());
    if (qMin(mbrect.width(), mbrect.height()) < qreal(1.0))
        return;

    qreal itemPenWidth;
    switch (item->type()) {
        case QGraphicsEllipseItem::Type:
            itemPenWidth = static_cast<QGraphicsEllipseItem *>(item)->pen().widthF();
            break;
        case QGraphicsPathItem::Type:
            itemPenWidth = static_cast<QGraphicsPathItem *>(item)->pen().widthF();
            break;
        case QGraphicsPolygonItem::Type:
            itemPenWidth = static_cast<QGraphicsPolygonItem *>(item)->pen().widthF();
            break;
        case QGraphicsRectItem::Type:
            itemPenWidth = static_cast<QGraphicsRectItem *>(item)->pen().widthF();
            break;
        case QGraphicsSimpleTextItem::Type:
            itemPenWidth = static_cast<QGraphicsSimpleTextItem *>(item)->pen().widthF();
            break;
        case QGraphicsLineItem::Type:
            itemPenWidth = static_cast<QGraphicsLineItem *>(item)->pen().widthF();
            break;
        default:
            itemPenWidth = 1.0;
    }
    const qreal pad = itemPenWidth / 2;

    const qreal penWidth = 0; // cosmetic pen

    const QColor fgcolor = option->palette.windowText().color();
    const QColor bgcolor( // ensure good contrast against fgcolor
        fgcolor.red()   > 127 ? 0 : 255,
        fgcolor.green() > 127 ? 0 : 255,
        fgcolor.blue()  > 127 ? 0 : 255);

    // Item外界矩形的边框颜色线
#if 0
    painter->setPen(QPen(bgcolor, penWidth, Qt::SolidLine));
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(item->boundingRect().adjusted(-pad, -pad, pad, pad));
#else
    painter->setPen(QPen(QColor("lightskyblue"), 0, Qt::SolidLine));
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(item->boundingRect().adjusted(-pad, -pad, pad, pad));
#endif
}

}// namespace gvf
