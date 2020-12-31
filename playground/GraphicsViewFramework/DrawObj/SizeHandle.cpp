#include "SizeHandle.h"
#include "DrawTool/DrawTool.h"
#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QPainter>
#include <qdebug.h>
#include <QtWidgets>
#include <QCursor>

namespace gvf {

SizeHandleRect::SizeHandleRect(QGraphicsItem* parent , int d, bool control)
    :QGraphicsRectItem(-SELECTION_HANDLE_SIZE/2,
                       -SELECTION_HANDLE_SIZE/2,
                       SELECTION_HANDLE_SIZE,
                       SELECTION_HANDLE_SIZE,parent)
    ,m_dir(d)
    ,m_controlPoint(control)
    ,m_state(SelectionHandleOff)
    ,m_penColor("black")
    ,m_brushColor("white")
    ,m_type(EightPointHandle)
{
    this->setAcceptHoverEvents(true);
    setFlag(QGraphicsItem::ItemIgnoresTransformations,true);
    hide();
}

// 绘制角点标注的样式
void SizeHandleRect::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();

    painter->setRenderHint(QPainter::Antialiasing);
    painter->setRenderHint(QPainter::SmoothPixmapTransform);

    if ( m_controlPoint  )
    {
        if (m_mouseEnter && DrawTool::c_drawShape == selection) {
            painter->setPen(QPen(m_penColor, 1, Qt::SolidLine)); // 绘制圆形外边框
            painter->setBrush(Qt::white);
            //painter->drawPoint(rect().center());
            painter->drawRect(rect().center().x()-8,
                              rect().center().y()-8,
                              16,16);
        } else {
            painter->setPen(Qt::NoPen); // 设置圆点无外边框
            painter->setBrush(m_brushColor);
            painter->drawEllipse(rect().center(),5,5); // 画圆形点
            //painter->drawPoint(rect().center()); // 画方形点
        }
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

    if (m_type == ShapeHandle) {
        show();
    }

    m_state = st;
}

void SizeHandleRect::move(qreal x, qreal y)
{   
    setPos(x,y);
}

void SizeHandleRect::setHandleType(SelectionHandleType type)
{
    m_type = type;
    m_type == EightPointHandle ? hide() : show();
}

void SizeHandleRect::hoverEnterEvent(QGraphicsSceneHoverEvent *e)
{
    if (m_controlPoint && DrawTool::c_drawShape == selection) {
        m_mouseEnter = true;
        m_oldCursor = cursor();
        setCursor(Qt::PointingHandCursor);
        update();
    }

    QGraphicsRectItem::hoverEnterEvent(e);
}

void SizeHandleRect::hoverLeaveEvent(QGraphicsSceneHoverEvent *e)
{
    if (m_controlPoint && DrawTool::c_drawShape == selection) {
        m_mouseEnter = false;
        setCursor(m_oldCursor);
        update();
    }

    QGraphicsRectItem::hoverLeaveEvent(e);
}

} // namespace gvf






