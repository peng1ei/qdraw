#pragma once
#include "GraphicsItem.h"

namespace gvf {

class GraphicsPolygonItem : public GraphicsItem
{
public:
    GraphicsPolygonItem(QGraphicsItem * parent = 0);
    QRectF boundingRect() const ;
    QPainterPath shape() const;
    virtual void addPoint( const QPointF & point ) ;
    virtual void endPoint(const QPointF & point );
    void control(int dir, const QPointF & delta);
    void stretch( int handle , double sx , double sy , const QPointF & origin );
    void updateCoordinate ();
    virtual bool loadFromXml(QXmlStreamReader * xml );
    virtual bool saveToXml( QXmlStreamWriter * xml );
    QString displayName() const { return tr("polygon"); }
    QGraphicsItem *duplicate() const;

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *e );
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *e );

protected:
    void updatehandles();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPolygonF m_points;
    QPolygonF m_initialPoints;
    int m_alpha = 50;
};

} // spacename gvf
