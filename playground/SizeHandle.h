#ifndef SIZEHANDLE
#define SIZEHANDLE

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


enum { SELECTION_HANDLE_SIZE = 6, SELECTION_MARGIN = 10 };
enum SelectionHandleState { SelectionHandleOff, SelectionHandleInactive, SelectionHandleActive };

// 外界矩形的角点和边框中点
enum { Handle_None = 0 , LeftTop , Top, RightTop, Right, RightBottom, Bottom, LeftBottom, Left };


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

    void setBrushColor(const QColor &color) { m_color = color; }

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *e );
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *e );
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    const int m_dir;
    bool   m_controlPoint;
    SelectionHandleState m_state;
    QColor m_color;
    bool m_mouseEnter = false;
    QCursor m_oldCursor;
};


#endif // SIZEHANDLE

