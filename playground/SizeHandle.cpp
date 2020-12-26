#include "SizeHandle.h"
#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QPainter>
#include <qdebug.h>
#include <QtWidgets>

SizeHandleRect::SizeHandleRect(QGraphicsItem* parent , int d, bool control)
    :QGraphicsRectItem(-SELECTION_HANDLE_SIZE/2,
                       -SELECTION_HANDLE_SIZE/2,
                       SELECTION_HANDLE_SIZE,
                       SELECTION_HANDLE_SIZE,parent)
    ,m_dir(d)
    ,m_controlPoint(control)
    ,m_state(SelectionHandleOff)
    ,borderColor("white")
{
    this->setAcceptHoverEvents(true);
    setFlag(QGraphicsItem::ItemIgnoresTransformations,true);
    hide();
}

// 绘制角点标注的样式
void SizeHandleRect::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();
    painter->setPen(Qt::SolidLine);
    painter->setBrush(QBrush(borderColor));

    //painter->setRenderHint(QPainter::Antialiasing,false);
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setRenderHint(QPainter::HighQualityAntialiasing);
    painter->setRenderHint(QPainter::SmoothPixmapTransform);

    if ( m_controlPoint  )
    {
        painter->setPen(Qt::NoPen); // 设置圆点无外边框

        //painter->setPen(QPen(Qt::red,0, Qt::SolidLine)); // 绘制圆形外边框
        painter->setBrush(Qt::green);
        //painter->drawEllipse(rect().center(),3,3);
        painter->drawEllipse(rect().center(),5,5); // 画圆形点
        //painter->drawPoint(rect().center()); // 画方形点
    }else {
        // 绘制多边形内部中间的那个白色矩形标志
        //painter->drawRect(rect());
    }

    painter->restore();
}


void SizeHandleRect::setState(SelectionHandleState st)
{
    if (st == m_state)
        return;
    switch (st) {
    case SelectionHandleOff:
        hide();
        break;
    case SelectionHandleInactive:
    case SelectionHandleActive:
        show();
        break;
    }
    borderColor = Qt::white;
    m_state = st;
}

void SizeHandleRect::move(qreal x, qreal y)
{   
    setPos(x,y);
}

void SizeHandleRect::hoverEnterEvent(QGraphicsSceneHoverEvent *e)
{
    borderColor = Qt::blue;
    update();
    QGraphicsRectItem::hoverEnterEvent(e);
}

void SizeHandleRect::hoverLeaveEvent(QGraphicsSceneHoverEvent *e)
{
    borderColor = Qt::white;
    update();
    QGraphicsRectItem::hoverLeaveEvent(e);
}







