#pragma once
#include "SizeHandle.h"
#include <QString>
#include <QPointF>
#include <QPen>
#include <QObject>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItem>
#include <QVector>
#include <QGraphicsScene>
#include <QCursor>
#include <QXmlStreamReader>
#include <QDebug>

namespace gvf {

#ifndef M_PI
#define M_PI 3.1415926
#endif

template < typename BaseType = QGraphicsItem >
class AbstractShapeType : public BaseType
{
public:
   explicit AbstractShapeType(QGraphicsItem * parent = 0 )
        :BaseType(parent)
    {
        m_pen = QPen(Qt::NoPen);
        m_width = m_height = 0;
    }
    virtual ~AbstractShapeType(){}
    virtual QString displayName () const { return QString("AbstractType");}
    virtual void control(int dir, const QPointF & delta ){ Q_UNUSED(dir);Q_UNUSED(delta);}
    virtual void stretch( int  , double  , double  , const QPointF & ) {}
    virtual QRectF  rect() const { return m_localRect; }
    virtual void updateCoordinate () {}
    virtual void move( const QPointF & point ){Q_UNUSED(point);}
    virtual QGraphicsItem * duplicate() const { return NULL;}
    virtual int handleCount() const { return m_handles.size();}
    virtual bool loadFromXml(QXmlStreamReader * xml ) = 0;
    virtual bool saveToXml( QXmlStreamWriter * xml ) = 0 ;
    int collidesWithHandle( const QPointF & point ) const
    {
        // 从最上层的控制点找
        const Handles::const_reverse_iterator hend =  m_handles.rend();
        for (Handles::const_reverse_iterator it = m_handles.rbegin(); it != hend; ++it)
        {
            QPointF pt = (*it)->mapFromScene(point);
            if ((*it)->contains(pt) ){
                return (*it)->dir();
            }
        }
        return Handle_None;
    }
    virtual QPointF handlePos( int handle ) const
    {
        const Handles::const_reverse_iterator hend =  m_handles.rend();
        for (Handles::const_reverse_iterator it = m_handles.rbegin(); it != hend; ++it)
        {
            if ((*it)->dir() == handle ){
                return (*it)->pos();
            }
        }
        return QPointF();
    }
    int swapHandle(int handle, const QPointF& scale ) const
    {
        int dir = Handle_None;
        if ( scale.x() < 0 || scale.y() < 0 ){
            switch (handle) {
            case RightTop:
                if ( scale.x() < 0 && scale.y() < 0 )
                    dir = LeftBottom;
                else if ( scale.x() > 0 && scale.y() < 0 )
                    dir = RightBottom;
                else
                    dir = LeftTop;
                break;
            case RightBottom:
                if ( scale.x() < 0 && scale.y() < 0 )
                    dir = LeftTop;
                else if ( scale.x() > 0 && scale.y() < 0 )
                    dir = RightTop;
                else
                    dir = LeftBottom;
                break;
            case LeftBottom:
                if ( scale.x() < 0 && scale.y() < 0 )
                    dir = RightTop;
                else if ( scale.x() > 0 && scale.y() < 0 )
                    dir = LeftTop;
                else
                    dir = RightBottom;
                break;
            case LeftTop:
                if ( scale.x() < 0 && scale.y() < 0 )
                    dir = RightBottom;
                else if ( scale.x() > 0 && scale.y() < 0 )
                    dir = LeftBottom;
                else
                    dir = RightTop;
                break;
            case Right:
                if (scale.x() < 0 )
                    dir = Left;
                break;
            case Left:
                if (scale.x() < 0 )
                    dir = Right;
                break;
            case Top:
                if (scale.y()<0)
                    dir = Bottom;
                break;
            case Bottom:
                if (scale.y()<0)
                   dir = Top;
                break;
            }
        }
        return dir;
    }

    QColor brushColor() const {return m_brush.color();}
    QBrush brush() const {return m_brush;}
    QPen   pen() const {return m_pen;}
    QColor penColor() const {return m_pen.color();}
    void   setPen(const QPen & pen ) { m_pen = pen;}
    void   setPenColor(const QColor &color) { m_pen.setColor(color); }
    void   setBrush( const QBrush & brush ) { m_brush = brush ; }
    void   setBrushColor( const QColor & color ) { m_brush.setColor(color);}
    qreal  width() const { return m_width ; }
    void   setWidth( qreal width )
    {
        m_width = width ;
        updateCoordinate();
    }
    qreal  height() const {return m_height;}
    void   setHeight ( qreal height )
    {
        m_height = height ;
        updateCoordinate();
    }

protected:
    virtual void updatehandles(){}
    void setState(SelectionHandleState st)
    {
        const Handles::iterator hend =  m_handles.end();
        for (Handles::iterator it = m_handles.begin(); it != hend; ++it)
            (*it)->setState(st);
    }
    QBrush m_brush;
    QPen   m_pen ;
    typedef std::vector<SizeHandleRect*> Handles;
    Handles m_handles;
    Handles m_shapeHandles;
    QRectF m_localRect;
    qreal m_width;
    qreal m_height;
};

typedef  AbstractShapeType< QGraphicsItem > AbstractShape;

} // end namespace gvf



