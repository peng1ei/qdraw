#ifndef GRAPHICSSCRIBBLEITEM_H
#define GRAPHICSSCRIBBLEITEM_H
#include "GraphicsItem.h"

namespace gvf {
    class GraphicsScribbleItem : public GraphicsItem
    {
    public:
        explicit GraphicsScribbleItem(qreal w, qreal h, QGraphicsItem * parent = 0);
        void setBeginPoint(const QPointF &point) { m_beginPoint = point; }
        void setEndPoint(const QPointF &point) { m_endPoint = point; }
        QRectF boundingRect() const ;
        QPainterPath shape() const;
        QGraphicsItem *duplicate() const;
        void control(int dir, const QPointF & delta);
        void updateCoordinate();
        virtual bool loadFromXml(QXmlStreamReader * xml ) { return true; }
        virtual bool saveToXml( QXmlStreamWriter * xml ) { return true; }
        QString displayName() const { return tr("scribble"); }
    protected:
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    private:
        qreal m_canvasWidth;
        qreal m_canvasHeight;
        qreal m_minX;
        qreal m_minY;
        qreal m_maxX;
        qreal m_maxY;
        QPointF m_beginPoint;
        QPointF m_endPoint;

        //原图
        QPixmap src;
        QPixmap out;
        //上一次的鼠标位置
        QPoint mpos;
        QPen pen;
        bool m_isScribbling = false;
    };
} // end namespace gvf
#endif // GRAPHICSSCRIBBLEITEM_H
