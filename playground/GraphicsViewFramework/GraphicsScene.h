#pragma once
#include <QGraphicsScene>
#include "DrawTool/DrawTool.h"
#include "DrawObj/DrawObj"

QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
class QMenu;
class QPointF;
class QGraphicsLineItem;
class QFont;
class QGraphicsTextItem;
class QColor;
class QKeyEvent;
class QPainter;
QT_END_NAMESPACE

namespace gvf {

class GraphicsView;

enum AlignType
{
    UP_ALIGN=0,
    HORZ_ALIGN,
    VERT_ALIGN,
    DOWN_ALIGN,
    LEFT_ALIGN,
    RIGHT_ALIGN,
    CENTER_ALIGN,
    HORZEVEN_ALIGN,
    VERTEVEN_ALIGN,
    WIDTH_ALIGN,
    HEIGHT_ALIGN,
    ALL_ALIGN
};

class GridTool
{
public:
    GridTool(const QSize &grid = QSize(3200,2400) , const QSize & space = QSize(20,20) );
    void paintGrid(QPainter *painter,const QRect & rect );
protected:
    QSize m_sizeGrid;
    QSize m_sizeGridSpace;
};

class GraphicsItemGroup;

class GraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    using Layer = QGraphicsItem;//QGraphicsItemGroup

    explicit GraphicsScene(QObject *parent = 0);
    ~GraphicsScene();
    void setView(QGraphicsView * view ) { m_view = view ; }
    QGraphicsView * view() { return m_view; }
    void align(AlignType alignType );
    void mouseEvent(QGraphicsSceneMouseEvent *mouseEvent );
    GraphicsItemGroup * createGroup(const QList<QGraphicsItem *> &items ,bool isAdd = true);
    void destroyGroup(QGraphicsItemGroup *group);

    void SetCurrentLayer(Layer *layer);
    Layer* curLayer() { return m_curLayer; }
    void addToCurLayer(QGraphicsItem *item);

signals:
    void itemMoved( QGraphicsItem * item , const QPointF & oldPosition );
    void itemRotate(QGraphicsItem * item , const qreal oldAngle );
    void itemAdded(QGraphicsItem * item );
    void itemResize(QGraphicsItem * item , int handle , const QPointF& scale );
    void itemControl(QGraphicsItem * item , int handle , const QPointF & newPos , const QPointF& lastPos_ );

protected:
    void drawBackground(QPainter *painter, const QRectF &rect) Q_DECL_OVERRIDE;
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvet) Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent *e) Q_DECL_OVERRIDE;
    void keyReleaseEvent(QKeyEvent *e) Q_DECL_OVERRIDE;
    QGraphicsView * m_view;

    qreal m_dx;
    qreal m_dy;
    bool  m_moved;
    GridTool *m_grid;

    Layer *m_curLayer = nullptr;

    friend class GraphicsView;
};

} // namespace gvf
