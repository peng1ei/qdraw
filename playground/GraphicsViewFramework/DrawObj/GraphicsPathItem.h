#ifndef GRAPHICSPATHITEM_H
#define GRAPHICSPATHITEM_H
#include "GraphicsItem.h"

namespace gvf {

class GraphicsPathItem : public GraphicsItem
{

public:
    GraphicsPathItem(GraphicsItem * parent = nullptr);

    void setBeginPoint(const QPointF &point);
    void setEndPoint(const QPointF &point);
    void setPenWidth(int w) { m_penWidth = w; }
    void setEraserWidth(int w) { m_eraserWidth = w; }
    void setEraser(bool value) { m_isEraser = value; }

    QRectF boundingRect() const;
    QPainterPath shape() const;

    QGraphicsItem *duplicate() const;
    void control(int dir, const QPointF & delta);
    void updateCoordinate();
    virtual bool loadFromXml(QXmlStreamReader * xml ) { return true; }
    virtual bool saveToXml( QXmlStreamWriter * xml ) { return true; }
    QString displayName() const { return tr("GraphicsPathItem"); }

protected:
    void updatehandles();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    qreal m_width;
    qreal m_height;
    QPointF m_beginPoint;
    QPointF m_endPoint;
    QRectF m_boundingRect;
    int m_penWidth;
    int m_eraserWidth;
    bool m_isEraser;
    QPainterPath *m_painterPath;
    QPainterPath *m_eraserPainterPath;
};

} // namespace gvf

#endif // GRAPHICSPATHITEM_H
