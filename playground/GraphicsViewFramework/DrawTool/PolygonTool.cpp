#include "PolygonTool.h"
#include "../GraphicsScene.h"

namespace gvf {

extern SelectMode selectMode;
extern int nDragHandle;

static PolygonTool lineTool(line);
static PolygonTool polygonTool(polygon);
static PolygonTool bezierTool(bezier);
static PolygonTool polylineTool(polyline);

PolygonTool::PolygonTool(DrawShape shape)
    :DrawTool(shape)
{
    item = nullptr;
    m_nPoints = 0;
}

void PolygonTool::mousePressEvent(QGraphicsSceneMouseEvent *event, GraphicsScene *scene)
{
    DrawTool::mousePressEvent(event,scene);

    if ( event->button() != Qt::LeftButton ) return;

    if ( item == nullptr ){
        if ( c_drawShape == polygon ){
        item = new GraphicsPolygonItem(NULL);
        }else if (c_drawShape == bezier ){
            item = new GraphicsBezier();
        }else if ( c_drawShape == polyline ){
            item = new GraphicsBezier(false);
        }else if ( c_drawShape == line ){
            item = new GraphicsLineItem(0);
        }
        item->setPenColor(DrawTool::c_penColor);
        item->setBrushColor(DrawTool::c_brushColor);
        item->setPos(event->scenePos());

        // TODO 加入当前图层
        scene->addItem(item);

        initialPositions = c_down;
        item->addPoint(c_down);
        item->setSelected(true);
        m_nPoints++;

    }else if ( c_down == c_last ){
        /*
        if ( item != NULL )
        {
            scene->removeItem(item);
            delete item;
            item = NULL ;
            c_drawShape = selection;
            selectMode = none;
            return ;
        }
        */
    }
    item->addPoint(c_down+QPoint(0,0)); //QPoint(1,0)
    m_nPoints++;
    selectMode = size ;
    nDragHandle = item->handleCount();
}

void PolygonTool::mouseMoveEvent(QGraphicsSceneMouseEvent *event, GraphicsScene *scene)
{
    DrawTool::mouseMoveEvent(event,scene);
    setCursor(scene,Qt::CrossCursor);

//    selectTool.mouseMoveEvent(event,scene);

    if ( item != 0 ){
        if ( nDragHandle != Handle_None && selectMode == size ){
            item->control(nDragHandle,c_last);
        }
    }

}

void PolygonTool::mouseReleaseEvent(QGraphicsSceneMouseEvent *event, GraphicsScene *scene)
{
    DrawTool::mousePressEvent(event,scene);
    if ( c_drawShape == line ){
        if (!item) return;

        item->endPoint(event->scenePos());
        item->updateCoordinate();
        emit scene->itemAdded( item );
        item = nullptr;
        selectMode = none;

        // TODO 在切换其他工具之前一直保持当前工具状态
        //c_drawShape = selection;

        m_nPoints = 0;

        scene->clearSelection();
    }

    scene->clearSelection();
}

void PolygonTool::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event, GraphicsScene *scene)
{
    DrawTool::mouseDoubleClickEvent(event,scene);
    item->endPoint(event->scenePos());
    item->updateCoordinate();
    emit scene->itemAdded( item );
    item = nullptr;
    selectMode = none;

    // TODO 在切换其他工具之前一直保持当前工具状态
    //c_drawShape = selection;

    m_nPoints = 0;

    // 会清除多边形的角点
    scene->clearSelection();
}

} // namespace gvf
