#include "DrawTool.h"
#include "DrawObj/DrawObj"
#include "../GraphicsScene.h"
#include <QGraphicsView>
#include <QGraphicsScene>

namespace gvf {

QList<DrawTool*> DrawTool::c_tools;
QPointF DrawTool::c_down;
QPointF DrawTool::c_last;
quint32 DrawTool::c_nDownFlags;

DrawShape DrawTool::c_drawShape = selection;

QColor DrawTool::c_penColor = Qt::green;
QColor DrawTool::c_brushColor = Qt::green;

//static PanTool panTool;
//static RubberBandZoomTool rubberBandZoomTool;
//static SelectTool selectTool;
//static RectTool   rectTool(rectangle);
//static RectTool   roundRectTool(roundrect);
//static RectTool   ellipseTool(ellipse);

//static PolygonTool lineTool(line);
//static PolygonTool polygonTool(polygon);
//static PolygonTool bezierTool(bezier);
//static PolygonTool polylineTool(polyline);

//static RotationTool rotationTool;

SelectMode selectMode = none;

// 用于指示控制的是外接矩形的哪个点
/**
 *      LeftTop     Top     RightTop
 *
 *      Left                Right
 *
 *      LeftBottom  Bottom  RightBottom
 */
int nDragHandle = Handle_None;

void setCursor(GraphicsScene * scene , const QCursor & cursor )
{
    QGraphicsView * view = scene->view();
    if (view) view->viewport()->setCursor(cursor);
}

DrawTool::DrawTool(DrawShape shape)
{
    m_drawShape = shape ;
    m_hoverSizer = false;
    c_tools.push_back(this);
}

void DrawTool::mousePressEvent(QGraphicsSceneMouseEvent *event, GraphicsScene *scene)
{
    Q_UNUSED(scene)
    c_down = event->scenePos();
    c_last = event->scenePos();
}

void DrawTool::mouseMoveEvent(QGraphicsSceneMouseEvent *event, GraphicsScene *scene)
{
    Q_UNUSED(scene)
    c_last = event->scenePos();
}

void DrawTool::mouseReleaseEvent(QGraphicsSceneMouseEvent *event, GraphicsScene *scene)
{
    Q_UNUSED(scene)
    if (event->scenePos() == c_down ) {
        // TODO 在切换其他工具之前一直保持当前工具状态
        //c_drawShape = selection;
    }
}

void DrawTool::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event, GraphicsScene *scene)
{
    Q_UNUSED(event)
    Q_UNUSED(scene)
}

DrawTool *DrawTool::findTool(DrawShape drawShape)
{
    QList<DrawTool*>::const_iterator iter = c_tools.constBegin();
    for ( ; iter != c_tools.constEnd() ; ++iter ){
        if ((*iter)->m_drawShape == drawShape )
            return (*iter);
    }
    return 0;
}

}// namespace gvf
