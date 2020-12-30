#include "RubberBandZoomTool.h"
#include "../GraphicsScene.h"
#include <QGraphicsView>

namespace gvf {

static RubberBandZoomTool rubberBandZoomTool;

RubberBandZoomTool::RubberBandZoomTool()
    :DrawTool(rubberbandzoom)
{
    dashRect = 0;
    selLayer = 0;
    opposite_ = QPointF();
}

void RubberBandZoomTool::mousePressEvent(QGraphicsSceneMouseEvent *event, GraphicsScene *scene)
{
    DrawTool::mousePressEvent(event,scene);
    scene->clearSelection();

    if ( event->button() != Qt::LeftButton ) return;

    nDragHandle = Handle_None;
    selectMode = none;

    if( selectMode == none ){
        selectMode = netSelect;
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

    setCursor(scene, QCursor(QPixmap(":/image/icons/zoomrectcur.png")));
}

void RubberBandZoomTool::mouseMoveEvent(QGraphicsSceneMouseEvent *event, GraphicsScene *scene)
{
    DrawTool::mouseMoveEvent(event,scene);
    scene->clearSelection();

    QList<QGraphicsItem *> items = scene->selectedItems();
    AbstractShape * item = 0;
    if ( items.count() == 1 ){
        item = qgraphicsitem_cast<AbstractShape*>(items.first());
        if ( item != 0 ){
            if ( nDragHandle != Handle_None && selectMode == size ){
                if (opposite_.isNull()){
                    //opposite_ = item->opposite(nDragHandle);
                    if( opposite_.x() == 0 )
                        opposite_.setX(1);
                    if (opposite_.y() == 0 )
                        opposite_.setY(1);
                }

                QPointF new_delta = item->mapFromScene(c_last) - opposite_;
                QPointF initial_delta = item->mapFromScene(c_down) - opposite_;

                double sx = new_delta.x() / initial_delta.x();
                double sy = new_delta.y() / initial_delta.y();

                item->stretch(nDragHandle, sx , sy , opposite_);

                emit scene->itemResize(item,nDragHandle,QPointF(sx,sy));

              //  qDebug()<<"scale:"<<nDragHandle<< item->mapToScene(opposite_)<< sx << " ，" << sy
              //         << new_delta << item->mapFromScene(c_last)
              //         << initial_delta << item->mapFromScene(c_down) << item->boundingRect();

            } else if ( nDragHandle > Left  && selectMode == editor ){
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

}

void RubberBandZoomTool::mouseReleaseEvent(QGraphicsSceneMouseEvent *event, GraphicsScene *scene)
{
    DrawTool::mouseReleaseEvent(event,scene);
    setCursor(scene, QCursor(QPixmap(":/image/icons/zoomrectcur.png")));

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
    opposite_ = QPointF();
    scene->mouseEvent(event);

    scene->clearSelection();

}

}// namespace gvf
