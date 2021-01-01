#include "RectTool.h"
#include "../GraphicsScene.h"
#include "SelectTool.h"
#include <QGraphicsView>

namespace gvf {

extern SelectMode selectMode;
extern int nDragHandle;
extern SelectTool selectTool;

static RectTool   rectTool(rectangle);
static RectTool   roundRectTool(roundrect);
static RectTool   ellipseTool(ellipse);

RectTool::RectTool(DrawShape drawShape)
    :DrawTool(drawShape)
{
    item = 0;
}

void RectTool::mousePressEvent(QGraphicsSceneMouseEvent *event, GraphicsScene *scene)
{
    qDebug() << "x scale: " << scene->view()->transform().m11();
    qDebug() << "y scale: " << scene->view()->transform().m22();

    if ( event->button() != Qt::LeftButton ) return;
    scene->clearSelection();

    DrawTool::mousePressEvent(event,scene);
    switch ( c_drawShape ){
    case rectangle:
        item = new GraphicsRectItem(QRectF(0,0,1,1));
        break;
    case roundrect:
        item = new GraphicsRectItem(QRectF(0,0,1,1),true);
        break;
    case ellipse:
        item = new GraphicsEllipseItem(QRect(0,0,1,1));
        break;
    default:
        break;
    }
    if ( item == 0) return;

    item->setPenColor(DrawTool::c_penColor);
    item->setBrushColor(DrawTool::c_brushColor);

    qDebug() << "c_down: " << c_down;
    //c_down+=QPoint(2,2);
    qDebug() << "c_down 2: " << c_down;

    qDebug() << "event->scenePos: " << event->scenePos();

    // 项目的位置在父坐标中描述其原点（本地坐标（0，0））
    // 即item在父坐标中的位置就是item坐标系的原点
    qDebug() << "curLayer: " <<scene->curLayer()->mapFromScene(event->scenePos());
    item->setPos( event->scenePos() );
    //scene->addItem(item);
    scene->addToCurLayer(item);

    qDebug() << "event->scenePos(): " << event->scenePos();
    qDebug() << "item event->scenePos(): " << item->mapFromScene(event->scenePos());

    item->setSelected(true);
    selectMode = size;
    nDragHandle = RightBottom;

    setCursor(scene, Qt::CrossCursor);
}

void RectTool::mouseMoveEvent(QGraphicsSceneMouseEvent *event, GraphicsScene *scene)
{
    selectTool.mouseMoveEvent(event,scene);
    setCursor(scene, Qt::CrossCursor);
}

void RectTool::mouseReleaseEvent(QGraphicsSceneMouseEvent *event, GraphicsScene *scene)
{
    //DrawTool::mouseMoveEvent(event,scene);
    selectTool.mouseReleaseEvent(event,scene);

    if ( event->scenePos() == (c_down/*-QPoint(2,2)*/)){

       if ( item != 0 ){
         item->setSelected(false);
         scene->removeItem(item);
         delete item ;
         item = 0;
       }
       qDebug()<<"RectTool removeItem:";
    }else if( item ){
        emit scene->itemAdded( item );
    }

    // TODO 在切换其他工具之前一直保持当前工具状态
    //c_drawShape = selection;

    scene->clearSelection();

    setCursor(scene, Qt::CrossCursor);
}

}// namespace gvf
