#include "RotationTool.h"
#include "../GraphicsScene.h"
#include "SelectTool.h"

namespace gvf {

extern SelectTool selectTool;

static RotationTool rotationTool;

RotationTool::RotationTool()
    :DrawTool(rotation)
{
    lastAngle = 0;
    dashRect = 0;
}

void RotationTool::mousePressEvent(QGraphicsSceneMouseEvent *event, GraphicsScene *scene)
{

    DrawTool::mousePressEvent(event,scene);
    if ( event->button() != Qt::LeftButton ) return;

    if (!m_hoverSizer)
      scene->mouseEvent(event);

    QList<QGraphicsItem *> items = scene->selectedItems();
    if ( items.count() == 1 ){
        AbstractShape * item = qgraphicsitem_cast<AbstractShape*>(items.first());
        if ( item != 0 ){
            nDragHandle = item->collidesWithHandle(event->scenePos());
            if ( nDragHandle !=Handle_None)
            {
                QPointF origin = item->mapToScene(item->boundingRect().center());

                qreal len_y = c_last.y() - origin.y();
                qreal len_x = c_last.x() - origin.x();

                qreal angle = atan2(len_y,len_x)*180/M_PI;

                lastAngle = angle;
                selectMode = rotate;

                if (dashRect ){
                    scene->removeItem(dashRect);
                    delete dashRect;
                    dashRect = 0;
                }

                dashRect = new QGraphicsPathItem(item->shape());
                dashRect->setPen(Qt::DashLine);
                dashRect->setPos(item->pos());
                dashRect->setTransformOriginPoint(item->transformOriginPoint());
                dashRect->setTransform(item->transform());
                dashRect->setRotation(item->rotation());
                dashRect->setScale(item->scale());
                dashRect->setZValue(item->zValue());
                scene->addItem(dashRect);
                setCursor(scene,QCursor((QPixmap(":/icons/rotate.png"))));

                return;
            }
            else{
                    // TODO 在切换其他工具之前一直保持当前工具状态
                    //c_drawShape = selection;

                    selectTool.mousePressEvent(event,scene);
                }
        }
    }

    setCursor(scene,Qt::ArrowCursor);
}

void RotationTool::mouseMoveEvent(QGraphicsSceneMouseEvent *event, GraphicsScene *scene)
{
    DrawTool::mouseMoveEvent(event,scene);
    QList<QGraphicsItem *> items = scene->selectedItems();
    if ( items.count() == 1 ){
        AbstractShape * item = qgraphicsitem_cast<AbstractShape*>(items.first());

        // 在旋转期间禁止移动
        item->setFlag(QGraphicsItem::ItemIsMovable, false);

        if ( item != 0  && nDragHandle !=Handle_None && selectMode == rotate ){

             QPointF origin = item->mapToScene(item->boundingRect().center());

             qreal len_y = c_last.y() - origin.y();
             qreal len_x = c_last.x() - origin.x();
             qreal angle = atan2(len_y,len_x)*180/M_PI;

             angle = item->rotation() + int(angle - lastAngle) ;

             if ( angle > 360 )
                 angle -= 360;
             if ( angle < -360 )
                 angle+=360;

             if ( dashRect ){
                 //dashRect->setTransform(QTransform::fromTranslate(15,15),true);
                 //dashRect->setTransform(QTransform().rotate(angle));
                 //dashRect->setTransform(QTransform::fromTranslate(-15,-15),true);
                 dashRect->setRotation( angle );
             }

             setCursor(scene,QCursor((QPixmap(":/image/icons/rotate.png"))));
        }
        else if ( item )
        {
            int handle = item->collidesWithHandle(event->scenePos());
            if ( handle != Handle_None){
                setCursor(scene,QCursor((QPixmap(":/image/icons/rotate.png"))));
                m_hoverSizer = true;
                return;
            }else{
                setCursor(scene,Qt::ArrowCursor);
                m_hoverSizer = false;
            }
        }
    }

    scene->mouseEvent(event);

    if (!m_hoverSizer)
        setCursor(scene,Qt::ArrowCursor);
}

void RotationTool::mouseReleaseEvent(QGraphicsSceneMouseEvent *event, GraphicsScene *scene)
{
    DrawTool::mouseReleaseEvent(event,scene);
    if ( event->button() != Qt::LeftButton ) return;

    QList<QGraphicsItem *> items = scene->selectedItems();
    if ( items.count() == 1 ){
        AbstractShape * item = qgraphicsitem_cast<AbstractShape*>(items.first());
        if ( item != 0  && nDragHandle !=Handle_None && selectMode == rotate ){

             QPointF origin = item->mapToScene(item->boundingRect().center());
             QPointF delta = c_last - origin ;
             qreal len_y = c_last.y() - origin.y();
             qreal len_x = c_last.x() - origin.x();
             qreal angle = atan2(len_y,len_x)*180/M_PI,oldAngle = item->rotation();
             angle = item->rotation() + int(angle - lastAngle) ;

             if ( angle > 360 )
                 angle -= 360;
             if ( angle < -360 )
                 angle+=360;

             item->setRotation( angle );
             emit scene->itemRotate(item , oldAngle);
             qDebug()<<"rotate:"<<angle<<item->boundingRect();
        }
    }

    setCursor(scene,Qt::ArrowCursor);
    selectMode = none;
    nDragHandle = Handle_None;
    lastAngle = 0;
    m_hoverSizer = false;
    if (dashRect ){
        scene->removeItem(dashRect);
        delete dashRect;
        dashRect = 0;
    }

    // 此句代码能够消除旋转后留下原始图形的外边框问题,
    //没有这句代码则需要重新单机item才能消除外边框
    selectTool.mouseReleaseEvent(event,scene);

    scene->mouseEvent(event);
}

}// namespace gvf
