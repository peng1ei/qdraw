#pragma once
#include <QGraphicsRectItem>
#include <QList>
#include <QCursor>

QT_BEGIN_NAMESPACE
class QColor;
class QFocusEvent;
class QGraphicsItem;
class QGraphicsScene;
class QGraphicsSceneMouseEvent;
class QGraphicsSceneHoverEvent;
QT_END_NAMESPACE

namespace gvf {

enum { SELECTION_HANDLE_SIZE = 6, SELECTION_MARGIN = 10 };
enum SelectionHandleState { SelectionHandleOff, SelectionHandleInactive, SelectionHandleActive };

// 外界矩形的角点和边框中点
enum {
    Handle_None = 0 ,
    LeftTop = 1 ,
    Top,
    RightTop,
    Right,
    RightBottom,
    Bottom,
    LeftBottom,
    Left,
    Handle_Shape = Left
};

enum SelectionHandleType { EightPointHandle = 0, SizeHandle, ShapeHandle };

/**
 *      LeftTop     Top     RightTop
 *
 *      Left                Right
 *
 *      LeftBottom  Bottom  RightBottom
 */
class SizeHandleRect :public QGraphicsRectItem
{
public:
    SizeHandleRect(QGraphicsItem* parent , int d , bool control = false );
    int dir() const  { return m_dir; }
    void setState(SelectionHandleState st);
    void move(qreal x, qreal y );

    void setHandleType(SelectionHandleType type);

    void setPenColor(const QColor &color) { m_penColor = color; }
    void setBrushColor(const QColor &color) { m_brushColor = color; }

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *e );
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *e );
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    const int m_dir;
    bool   m_controlPoint;
    SelectionHandleState m_state;
    QColor m_penColor;
    QColor m_brushColor;
    bool m_mouseEnter = false;
    QCursor m_oldCursor;
    SelectionHandleType m_type;
};

} // namespace gvf



