#ifndef DRAWTOOL
#define DRAWTOOL

#include "DrawObj.h"
#include "GraphicsScene.h"

QT_BEGIN_NAMESPACE
class QGraphicsScene;
class QGraphicsSceneMouseEvent;
class QMenu;
class QPointF;
class QGraphicsLineItem;
class QFont;
class QGraphicsTextItem;
class QColor;
class QAbstractGraphicsShapeItem;
QT_END_NAMESPACE

class GraphicsScene;
class GraphicsItem;
class GraphicsPolygonItem;
enum DrawShape
{
    selection ,
    rotation  ,
    line ,
    rectangle ,
    roundrect ,
    ellipse ,
    bezier,
    polygon,
    polyline,
    rubberbandzoom,
    pan
};

class DrawTool
{
public:
    DrawTool( DrawShape shape );
    virtual void mousePressEvent(QGraphicsSceneMouseEvent * event , GraphicsScene * scene ) ;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event , GraphicsScene * scene ) ;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * event , GraphicsScene * scene );
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event ,GraphicsScene *scene );
    DrawShape m_drawShape;
    bool m_hoverSizer;

    static DrawTool * findTool( DrawShape drawShape );
    static QList<DrawTool*> c_tools; // 全局绘图工具集合
    static QPointF c_down;
    static quint32 c_nDownFlags;
    static QPointF c_last;
    static DrawShape c_drawShape; // 当前绘制的Shape

    static QColor c_penColor; // 当前绘制的笔的颜色
    static QColor c_brushColor; // 当前绘制的画刷的颜色
};

class SelectTool : public DrawTool
{
public:
    SelectTool();
    virtual void mousePressEvent(QGraphicsSceneMouseEvent * event , GraphicsScene * scene ) ;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event , GraphicsScene * scene ) ;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * event , GraphicsScene * scene );
    QPointF initialPositions;
    QPointF opposite_;
    QGraphicsPathItem * dashRect;
    GraphicsItemGroup * selLayer;
};

class RubberBandZoomTool : public DrawTool
{
public:
    RubberBandZoomTool();
    virtual void mousePressEvent(QGraphicsSceneMouseEvent * event , GraphicsScene * scene ) ;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event , GraphicsScene * scene ) ;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * event , GraphicsScene * scene );
    QPointF initialPositions;
    QPointF opposite_;
    QGraphicsPathItem * dashRect;
    GraphicsItemGroup * selLayer;
};

class PanTool : public DrawTool
{
public:
    PanTool();
    virtual void mousePressEvent(QGraphicsSceneMouseEvent * event , GraphicsScene * scene ) ;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event , GraphicsScene * scene ) ;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * event , GraphicsScene * scene );
};

class  RotationTool : public DrawTool
{
public:
    RotationTool();
    virtual void mousePressEvent(QGraphicsSceneMouseEvent * event , GraphicsScene * scene ) ;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event , GraphicsScene * scene ) ;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * event , GraphicsScene * scene );
    qreal lastAngle;
    QGraphicsPathItem * dashRect;
};

class RectTool : public DrawTool
{
public:
    RectTool(DrawShape drawShape);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent * event , GraphicsScene * scene ) ;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event , GraphicsScene * scene ) ;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * event , GraphicsScene * scene );
    GraphicsItem * item;
};

class PolygonTool : public DrawTool
{
public:
    PolygonTool(DrawShape shape );
    virtual void mousePressEvent(QGraphicsSceneMouseEvent * event , GraphicsScene * scene ) ;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event , GraphicsScene * scene ) ;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * event , GraphicsScene * scene );
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event ,GraphicsScene *scene );
    GraphicsPolygonItem * item;
    int m_nPoints;
    QPointF initialPositions;
};

#endif // DRAWTOOL

