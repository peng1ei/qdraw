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
    ,m_penColor(78,182,243)
    ,m_brushColor(78,182,243)
    ,m_type(EightPointHandle)
{
    this->setAcceptHoverEvents(true);
    setFlag(QGraphicsItem::ItemIgnoresTransformations,true);
    hide();
}

// 绘制角点标注的样式
void SizeHandleRect::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->save();

    painter->setRenderHint(QPainter::Antialiasing);
    painter->setRenderHint(QPainter::SmoothPixmapTransform);

    if ( m_controlPoint  )
    {
        int size = 4;
        if (m_dir <= Left)
            size = 8;

        if (m_mouseEnter && DrawTool::c_drawShape == selection) {
            painter->setPen(QPen(m_penColor, 2, Qt::SolidLine)); // 绘制圆形外边框
            painter->setBrush(Qt::white);
            //painter->drawPoint(rect().center());
            painter->drawRect(rect().center().x()-size,
                              rect().center().y()-size,
                              2*size,2*size);
        } else {
            painter->setPen(Qt::NoPen); // Qt::NoPen -- 设置圆点无外边框
            painter->setBrush(m_penColor);

            painter->drawEllipse(rect().center(),size,size); // 画圆形点
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
    // TODO 是否需要加 m_dir判断???
    if (m_controlPoint && DrawTool::c_drawShape == selection && m_dir <= Left) {
        m_mouseEnter = true;
        m_oldCursor = cursor();
        setCursor(Qt::PointingHandCursor);
        update();
    }

    QGraphicsRectItem::hoverEnterEvent(e);
}

void SizeHandleRect::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    if (m_controlPoint && DrawTool::c_drawShape == selection && m_dir <= Left) {
        setCursor(Qt::PointingHandCursor);
        update();
    }

    QGraphicsRectItem::hoverMoveEvent(event);
}

void SizeHandleRect::hoverLeaveEvent(QGraphicsSceneHoverEvent *e)
{
    if (m_controlPoint && DrawTool::c_drawShape == selection && m_dir <= Left) {
        m_mouseEnter = false;
        setCursor(m_oldCursor);
        update();
    }

    QGraphicsRectItem::hoverLeaveEvent(e);
}

} // namespace gvf






