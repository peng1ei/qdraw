#include "SelectTool.h"
#include "../GraphicsScene.h"
#include <QGraphicsView>

namespace gvf {

SelectTool selectTool;

SelectTool::SelectTool()
    :DrawTool(selection)
{
    dashRect = 0;
    selLayer = 0;
    //opposite_ = QPointF();
}

void SelectTool::mousePressEvent(QGraphicsSceneMouseEvent *event, GraphicsScene *scene)
{
    DrawTool::mousePressEvent(event,scene);

    if ( event->button() != Qt::LeftButton ) return;

    // TODO m_hoverSizer什么意思？？？
    if (!m_hoverSizer)
       scene->mouseEvent(event);

    nDragHandle = Handle_None;
    selectMode = none;
    QList<QGraphicsItem *> items = scene->selectedItems();
    AbstractShape *item = 0;

    if ( items.count() == 1 )
        item = qgraphicsitem_cast<AbstractShape*>(items.first());

    if ( item != 0 ){
        // 碰撞检测，检测当前鼠标点所在的控制点的位置
        nDragHandle = item->collidesWithHandle(event->scenePos());

        // 当前鼠标点在预定义的控制点内（LeftTop ... Left）
        if ( nDragHandle != Handle_None && nDragHandle <= Left )
             selectMode = editor; // size, 原先是size，但是目前size好像都没用到了
        else if ( nDragHandle > Left ) // 自定义的控制点
            selectMode = editor;
        else // nDragHandle == Handle_None
            selectMode =  move;

        // 如果当前控制点在与控制点内，则计算当前控制点的相对位置的控制点
        if ( nDragHandle!= Handle_None && nDragHandle <= Left ){
//            opposite_ = item->opposite(nDragHandle);
//            if( opposite_.x() == 0 )
//                opposite_.setX(1);
//            if (opposite_.y() == 0 )
//                opposite_.setY(1);
        }

        setCursor(scene,Qt::ClosedHandCursor);
    } else if ( items.count() > 1 )
        selectMode =  move; // 如果当前选中多个items，则认为是要移动items

    // 当前没有选中任何item
    if( selectMode == none ){
        selectMode = netSelect; // 选择多个？？？
        if ( scene->view() ){
            QGraphicsView * view = scene->view();
            view->setDragMode(QGraphicsView::RubberBandDrag);
        }
#if 0
        if ( selLayer ){
            scene->destroyGroup(selLayer);
            selLayer = 0;
        }
#endif
    }

    if ( selectMode == move && items.count() == 1 ){

        if (dashRect ){
            scene->removeItem(dashRect);
            delete dashRect;
            dashRect = 0;
        }

        dashRect = new QGraphicsPathItem(item->shape());
        QPen tpen = dashRect->pen();
        tpen.setWidthF(2);
        tpen.setStyle(Qt::DashLine);
        tpen.setColor(Qt::white);
        tpen.setCosmetic(true);

        dashRect->setPen(tpen);
        dashRect->setPos(item->pos());
        dashRect->setTransformOriginPoint(item->transformOriginPoint());
        dashRect->setTransform(item->transform());
        dashRect->setRotation(item->rotation());
        dashRect->setScale(item->scale());
        dashRect->setZValue(item->zValue());

        scene->addItem(dashRect);

        initialPositions = item->pos();
    }

    if (DrawTool::c_drawShape == selection)
        setCursor(scene, Qt::ArrowCursor);
}

void SelectTool::mouseMoveEvent(QGraphicsSceneMouseEvent *event, GraphicsScene *scene)
{
    DrawTool::mouseMoveEvent(event,scene);

    QList<QGraphicsItem *> items = scene->selectedItems();
    AbstractShape * item = 0;

    if ( items.count() == 1 ){
        item = qgraphicsitem_cast<AbstractShape*>(items.first());

        if ( item != 0 ){
            if ( nDragHandle != Handle_None && selectMode == size ){
                QPointF opposite_(0,0);
                QPointF new_delta = item->mapFromScene(c_last) - opposite_;
                QPointF initial_delta = item->mapFromScene(c_down) - opposite_;
                if (initial_delta.x() == 0 && initial_delta.y()==0) {
                    initial_delta.setX(1);
                    initial_delta.setY(1);
                }

                double sx = new_delta.x();// / initial_delta.x();
                double sy = new_delta.y();// / initial_delta.y();

                item->stretch(nDragHandle, sx , sy , opposite_);
                emit scene->itemResize(item,nDragHandle,QPointF(sx,sy));

              //  qDebug()<<"scale:"<<nDragHandle<< item->mapToScene(opposite_)<< sx << " ，" << sy
              //         << new_delta << item->mapFromScene(c_last)
              //         << initial_delta << item->mapFromScene(c_down) << item->boundingRect();

            } else if ( nDragHandle > Left  && selectMode == editor ){
                item->control(nDragHandle,c_last);
                emit scene->itemControl(item,nDragHandle,c_last,c_down);
            }
            else if ( nDragHandle <= Left && nDragHandle > Handle_None  && selectMode == editor ){
                            item->control(nDragHandle,c_last);
                            emit scene->itemControl(item,nDragHandle,c_last,c_down);
            }
            else if(nDragHandle == Handle_None ){
                 int handle = item->collidesWithHandle(event->scenePos());
                 if ( handle != Handle_None){
                     //在绘制状态中鼠标全部为十字丝状态
                     //setCursor(scene,Qt::OpenHandCursor);
                     m_hoverSizer = true;
                 }else{
                     //setCursor(scene,Qt::ArrowCursor);
                     m_hoverSizer = false;
                 }
             }
        }
    }

    if ( selectMode == move ){
        //setCursor(scene,Qt::ClosedHandCursor);
        if ( dashRect ){
            dashRect->setPos(initialPositions + c_last - c_down);
        }
    }

    if ( selectMode != size  && items.count() > 1)
    {
        scene->mouseEvent(event);
    }

    if (DrawTool::c_drawShape == selection) {
        setCursor(scene, Qt::ArrowCursor);
    }
}

void SelectTool::mouseReleaseEvent(QGraphicsSceneMouseEvent *event, GraphicsScene *scene)
{
    DrawTool::mouseReleaseEvent(event,scene);

    if ( event->button() != Qt::LeftButton ) return;

    QList<QGraphicsItem *> items = scene->selectedItems();
    if ( items.count() == 1 ){
        AbstractShape * item = qgraphicsitem_cast<AbstractShape*>(items.first());
        if ( item != 0  && selectMode == move && c_last != c_down ){
             item->setPos(initialPositions + c_last - c_down);
             emit scene->itemMoved(item , c_last - c_down );
         }else if ( item !=0 && (selectMode == size || selectMode ==editor) && c_last != c_down ){

            item->updateCoordinate();
        }
    }else if ( items.count() > 1 && selectMode == move && c_last != c_down ){
          emit scene->itemMoved(NULL , c_last - c_down );
    }

    if (selectMode == netSelect ){

        if ( scene->view() ){
            QGraphicsView * view = scene->view();
            view->setDragMode(QGraphicsView::NoDrag);
        }
#if 0
        if ( scene->selectedItems().count() > 1 ){
            selLayer = scene->createGroup(scene->selectedItems());
            selLayer->setSelected(true);
        }
#endif
    }
    if (dashRect ){
        scene->removeItem(dashRect);
        delete dashRect;
        dashRect = 0;
    }
    selectMode = none;
    nDragHandle = Handle_None;
    m_hoverSizer = false;
    //opposite_ = QPointF();
    scene->mouseEvent(event);

    if (DrawTool::c_drawShape == selection)
        setCursor(scene, Qt::ArrowCursor);
}

}// namespace gvf
