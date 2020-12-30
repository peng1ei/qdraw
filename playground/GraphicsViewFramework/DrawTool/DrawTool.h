#pragma once
#include <QObject>

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
class QGraphicsPathItem;
QT_END_NAMESPACE

namespace gvf {

class GraphicsScene;
class GraphicsItem;
class GraphicsPolygonItem;
class GraphicsItemGroup;

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

enum SelectMode
{
    none,
    netSelect,
    move,
    size,
    rotate,
    editor,
};

extern SelectMode selectMode;
extern int nDragHandle;

void setCursor(GraphicsScene * scene , const QCursor & cursor );

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
    static QPointF c_down; // mouse press时的坐标
    static quint32 c_nDownFlags; // 标志当前是哪个控制点
    static QPointF c_last; // mouse move时的坐标
    static DrawShape c_drawShape; // 当前绘制的Shape

    static QColor c_penColor; // 当前绘制的笔的颜色
    static QColor c_brushColor; // 当前绘制的画刷的颜色
};

} // namespace gvf



