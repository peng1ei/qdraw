#pragma once
#include "AbstractShapeType.hpp"
#include <QStyleOptionGraphicsItem>
#include <QPainter>

namespace gvf {

class GraphicsItem : public QObject,
        public AbstractShapeType<QGraphicsItem>
{
    Q_OBJECT
    Q_PROPERTY(QColor pen READ penColor WRITE setPen )
    Q_PROPERTY(QColor brush READ brushColor WRITE setBrushColor )
    Q_PROPERTY(qreal  width READ width WRITE setWidth )
    Q_PROPERTY(qreal  height READ height WRITE setHeight )
    Q_PROPERTY(QPointF  position READ pos WRITE setPos )

public:
    GraphicsItem(QGraphicsItem * parent );
    enum {Type = UserType+1};
    int  type() const { return Type; }
    virtual QPixmap image() ;
signals:
    void selectedChange(QGraphicsItem *item);

protected:
    QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value);
    void updatehandles();
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

    bool readBaseAttributes(QXmlStreamReader * xml );
    bool writeBaseAttributes( QXmlStreamWriter * xml );
};

QRectF RecalcBounds(const QPolygonF&  pts);
QPainterPath qt_graphicsItem_shapeFromPath(const QPainterPath &path, const QPen &pen);
void qt_graphicsItem_highlightSelected(
    QGraphicsItem *item, QPainter *painter, const QStyleOptionGraphicsItem *option);

}// namespace gvf


