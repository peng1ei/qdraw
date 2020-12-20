#include "GraphicsView.h"

#include <QWheelEvent>
#include <QOpenGLWidget>
#include <QGLWidget>
#include <QGLFormat>
#include <QDebug>
#include <qmath.h>

#define VIEW_CENTER viewport()->rect().center()
#define VIEW_WIDTH  viewport()->rect().width()
#define VIEW_HEIGHT viewport()->rect().height()
#define PAN_BUTTON Qt::MidButton

class InteractiveViewPrivate {
public:
    bool mIsPan = false;
    QPoint mLastMousePos;

    double mScale = 1.0;
    double mZoomFactorBase = 1.0025;

    QPointF mTargetScenePos, mTargetViewportPos;
};

InteractiveView::InteractiveView(QWidget *parent)
    : QGraphicsView(parent), d_ptr(new InteractiveViewPrivate) {

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setCursor(Qt::ArrowCursor);
    setRenderHint(QPainter::Antialiasing);
    //setDragMode(QGraphicsView::RubberBandDrag);

    setSceneRect(INT_MIN/2, INT_MIN/2, INT_MAX, INT_MAX);
    centerOn(0, 0);
    //setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
    setCacheMode(QGraphicsView::CacheBackground);
    setViewportUpdateMode( QGraphicsView::FullViewportUpdate );
    setMouseTracking(true);
    setBackgroundBrush(QBrush(Qt::darkGray));
}

InteractiveView::~InteractiveView()
{
    delete d_ptr;
    d_ptr = nullptr;
}

void InteractiveView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == PAN_BUTTON) {
        // TODO 当光标底下没有 item 时才能移动
        //QPointF point = mapToScene(event->pos());
        //if (true || scene()->itemAt(point, transform()) == NULL)  {
            d_ptr->mIsPan = true;
            d_ptr->mLastMousePos = event->pos();

            setCursor(Qt::ClosedHandCursor);
        //}
    }

    QGraphicsView::mousePressEvent(event);
}

void InteractiveView::mouseMoveEvent(QMouseEvent *event)
{
    QPointF delta = d_ptr->mTargetViewportPos - event->pos();
    if (qAbs(delta.x()) > 0/*5*/ || qAbs(delta.y()) > 0/*5*/) {
        d_ptr->mTargetViewportPos = event->pos();
        d_ptr->mTargetScenePos = mapToScene(event->pos());
    }

    if (d_ptr->mIsPan){
        QPointF mouseDelta = mapToScene(event->pos()) - mapToScene(d_ptr->mLastMousePos);
        Pan(mouseDelta);
        d_ptr->mLastMousePos = event->pos();
    }

    QGraphicsView::mouseMoveEvent(event);
}

void InteractiveView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == PAN_BUTTON) {
        d_ptr->mIsPan = false;
        setCursor(Qt::ArrowCursor);
    }

    QGraphicsView::mouseReleaseEvent(event);
}

// 放大/缩小
// 参考：https://qa.1r1g.com/sf/ask/1337947271/ QGraphicsView使用鼠标滚轮在鼠标位置放大和缩小
// https://mlog.club/article/2035896
// https://stackoverflow.com/questions/19113532/qgraphicsview-zooming-in-and-out-under-mouse-position-using-mouse-wheel
void InteractiveView::wheelEvent(QWheelEvent *event)
{
    double angle = event->angleDelta().y();
    double factor = qPow(d_ptr->mZoomFactorBase, angle);

    Zoom(factor);
}

void InteractiveView::Pan(QPointF delta)
{
    // 根据当前 zoom 缩放平移数
    delta *= d_ptr->mScale;

    // view 根据鼠标下的点作为锚点来定位 scene
    //setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    QPoint newCenter(VIEW_WIDTH / 2 - delta.x(),  VIEW_HEIGHT / 2 - delta.y());
    centerOn(mapToScene(newCenter));

    // scene 在 view 的中心点作为锚点
    //setTransformationAnchor(QGraphicsView::AnchorViewCenter);
}

void InteractiveView::Zoom(double factor)
{
    scale(factor, factor);
    centerOn(d_ptr->mTargetScenePos);

    QPointF delta_viewport_pos = d_ptr->mTargetViewportPos - QPointF(viewport()->width() / 2.0,
                                                               viewport()->height() / 2.0);
    QPointF viewport_center = mapFromScene(d_ptr->mTargetScenePos) - delta_viewport_pos;
    centerOn(mapToScene(viewport_center.toPoint()));

    d_ptr->mScale *= factor;

    emit scaleChanged(d_ptr->mScale);
}
