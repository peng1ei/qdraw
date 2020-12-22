#include "GraphicsView.h"
#include "RuleBar.h"
#include "DrawTool.h"

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
    void SetRuleBarVisiable(bool value);

    bool mIsPan = false;
    QPoint mLastMousePos;
    double mScale = 1.0;
    double mZoomFactorBase = 1.0025;
    QPointF mTargetScenePos, mTargetViewportPos;

    bool mRuleBarVisiable = true;
    QtRuleBar *mHRuler = nullptr;
    QtRuleBar *mVRuler = nullptr;
    QtCornerBox *mBox = nullptr;

    QRect mRubberBandRect;
};

InteractiveView::InteractiveView(QWidget *parent)
    : QGraphicsView(parent), d_ptr(new InteractiveViewPrivate) {

#if 1
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
#endif
    setCursor(Qt::ArrowCursor);
    setRenderHint(QPainter::Antialiasing);
    //setDragMode(QGraphicsView::RubberBandDrag);

    // TODO
    setSceneRect(INT_MIN/2, INT_MIN/2, INT_MAX, INT_MAX);
    centerOn(0, 0);
    //setTransformationAnchor(QGraphicsView::AnchorViewCenter);

    setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers | QGL::DoubleBuffer)));
    setCacheMode(QGraphicsView::CacheBackground);
    setViewportUpdateMode( QGraphicsView::FullViewportUpdate );
    setMouseTracking(true);
    setBackgroundBrush(QBrush(Qt::darkGray));

    if (d_ptr->mRuleBarVisiable) {
        d_ptr->mHRuler = new QtRuleBar(Qt::Horizontal,this,this);
        d_ptr->mVRuler = new QtRuleBar(Qt::Vertical,this,this);
        d_ptr->mBox = new QtCornerBox(this);
    }    
}

InteractiveView::~InteractiveView()
{
    delete d_ptr;
    d_ptr = nullptr;
}

void InteractiveView::SetRuleBarVisiable(bool value)
{
    d_ptr->SetRuleBarVisiable(value);    
    ResizeRuler();    
    update();
}

void InteractiveView::FitInView(const QRectF &rect, Qt::AspectRatioMode aspectRadioMode)
{
    fitInView(rect,aspectRadioMode);
    
    d_ptr->mScale = transform().m11(); // x scale, m12 -> y scale, default 0
    emit scaleChanged(d_ptr->mScale);
}

void InteractiveView::FitInView(qreal x, qreal y, qreal w, qreal h, Qt::AspectRatioMode aspectRadioMode)
{
    fitInView(x,  y, w, h, aspectRadioMode);
    d_ptr->mScale = transform().m11(); // x scale, m12 -> y scale, default 0
    emit scaleChanged(d_ptr->mScale);
}

void InteractiveView::FitInView(const QGraphicsItem *item, Qt::AspectRatioMode aspectRadioMode)
{
    fitInView(item, aspectRadioMode);
    d_ptr->mScale = transform().m11(); // x scale, m12 -> y scale, default 0
    emit scaleChanged(d_ptr->mScale);
}

void InteractiveView::ZoomInUnderViewCenter()
{
    auto pos = QPoint(viewport()->rect().width()/2, viewport()->height()/2);

    d_ptr->mTargetViewportPos = pos;
    d_ptr->mTargetScenePos = mapToScene(pos);
    Zoom(1.25);
}

void InteractiveView::ZoomOutUnderViewCenter()
{
    auto pos = QPoint(viewport()->rect().width()/2, viewport()->height()/2);

    d_ptr->mTargetViewportPos = pos;
    d_ptr->mTargetScenePos = mapToScene(pos);
    Zoom(1/1.25);
}

void InteractiveView::Zoom1To1()
{
    // scale to 1:1
    QRectF unity = transform().mapRect(QRectF(0, 0, 1, 1));
    if (unity.isEmpty())
        return;
    scale(1 / unity.width(), 1 / unity.height());

    centerOn((QRectF(0,0,960,720)).center());
    d_ptr->mScale = 1;
    emit scaleChanged(d_ptr->mScale);
}

void InteractiveView::ZoomToRect()
{

}

void InteractiveView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == PAN_BUTTON) {
        d_ptr->mIsPan = true;
        d_ptr->mLastMousePos = event->pos();
        
        setCursor(Qt::ClosedHandCursor);
    }

    QGraphicsView::mousePressEvent(event);
}

void InteractiveView::mouseMoveEvent(QMouseEvent *event)
{
    QPointF delta = d_ptr->mTargetViewportPos - event->pos();
    if (qAbs(delta.x()) > 0 || qAbs(delta.y()) > 0) {
        d_ptr->mTargetViewportPos = event->pos();
        d_ptr->mTargetScenePos = mapToScene(event->pos());
    }

    if (d_ptr->mIsPan){
        QPointF mouseDelta = mapToScene(event->pos()) - mapToScene(d_ptr->mLastMousePos);
        Pan(mouseDelta);
        d_ptr->mLastMousePos = event->pos();
    }

    if (d_ptr->mRuleBarVisiable) {
        // 刻度尺
        //QPointF pt = mapToScene(event->pos());
        d_ptr->mHRuler->updatePosition(event->pos());
        d_ptr->mVRuler->updatePosition(event->pos());
        //emit positionChanged( pt.x() , pt.y() );
    }
    
    
#if 0
    d_ptr->mLastMousePos = event->pos();
    this->viewport()->update();
#endif

    QGraphicsView::mouseMoveEvent(event);
    
    if (d_ptr->mIsPan){
        setCursor(Qt::ClosedHandCursor);
    }

    emit posFromSceneChanged(d_ptr->mTargetScenePos.x(), d_ptr->mTargetScenePos.y());
    emit posFromViewChanged(event->x(), event->y());

    d_ptr->mRubberBandRect = rubberBandRect();
    qDebug() << "Rubber band Rect wheel: " << d_ptr->mRubberBandRect;
}

void InteractiveView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == PAN_BUTTON) {
        d_ptr->mIsPan = false;
        setCursor(Qt::ArrowCursor);
    }

    if (DrawTool::c_drawShape == rubberbandzoom) {
        // TODO判断mRubberBandRect的有效性
        FitInView(d_ptr->mRubberBandRect, Qt::KeepAspectRatio);
    }

    QGraphicsView::mouseReleaseEvent(event);

    qDebug() << "Rubber band Rect release: " << rubberBandRect();
}

void InteractiveView::resizeEvent(QResizeEvent *event)
{
    QGraphicsView::resizeEvent(event);

    if (d_ptr->mRuleBarVisiable) {
        this->setViewportMargins(RULER_SIZE-1,RULER_SIZE-1,0,0);
        d_ptr->mHRuler->resize(this->size().width()- RULER_SIZE - 1,RULER_SIZE);
        d_ptr->mHRuler->move(RULER_SIZE,0);
        d_ptr->mVRuler->resize(RULER_SIZE,this->size().height() - RULER_SIZE - 1);
        d_ptr->mVRuler->move(0,RULER_SIZE);

        d_ptr->mBox->resize(RULER_SIZE,RULER_SIZE);
        d_ptr->mBox->move(0,0);
        UpdateRuler();
    }

}

void InteractiveView::scrollContentsBy(int dx, int dy)
{
    QGraphicsView::scrollContentsBy(dx,dy);

    if (d_ptr->mRuleBarVisiable)
        UpdateRuler();
}

void InteractiveView::paintEvent(QPaintEvent *e)
{
    QGraphicsView::paintEvent(e);
    
#if 0
    qDebug() << "InteractiveView::paintEvent";

    QPainter painter(this->viewport());
    QPen pen;
    pen.setWidth(1);
    pen.setColor(Qt::red);
    painter.setPen(pen);
    //绘制横向线
    painter.drawLine(0, d_ptr->mLastMousePos.y(), width(), d_ptr->mLastMousePos.y());
    //绘制纵向线
    painter.drawLine(d_ptr->mLastMousePos.x(), 0, d_ptr->mLastMousePos.x(), height());
#endif
}

void InteractiveView::UpdateRuler()
{
    if ( scene() == 0) return;
    QRectF viewbox = this->rect();
    QPointF offset = mapFromScene(scene()->sceneRect().topLeft());
    double factor =  1./transform().m11();
    double lower_x = factor * ( viewbox.left()  - offset.x() );
    double upper_x = factor * ( viewbox.right() -RULER_SIZE- offset.x()  );
    d_ptr->mHRuler->setRange(lower_x,upper_x,upper_x - lower_x );
    d_ptr->mHRuler->update();

    double lower_y = factor * ( viewbox.top() - offset.y()) * -1;
    double upper_y = factor * ( viewbox.bottom() - RULER_SIZE - offset.y() ) * -1;

    d_ptr->mVRuler->setRange(lower_y,upper_y,upper_y - lower_y );
    d_ptr->mVRuler->update();
}

void InteractiveView::ResizeRuler()
{
    if (d_ptr->mRuleBarVisiable) {
        this->setViewportMargins(RULER_SIZE-1,RULER_SIZE-1,0,0);
        d_ptr->mHRuler->resize(this->size().width()- RULER_SIZE - 1,RULER_SIZE);
        d_ptr->mHRuler->move(RULER_SIZE,0);
        d_ptr->mVRuler->resize(RULER_SIZE,this->size().height() - RULER_SIZE - 1);
        d_ptr->mVRuler->move(0,RULER_SIZE);

        d_ptr->mBox->resize(RULER_SIZE,RULER_SIZE);
        d_ptr->mBox->move(0,0);
        UpdateRuler();
    } else {
        this->setViewportMargins(0,0,0,0);
    }
}

// 放大/缩小
// 参考：https://qa.1r1g.com/sf/ask/1337947271/ QGraphicsView使用鼠标滚轮在鼠标位置放大和缩小
// https://mlog.club/article/2035896
// https://stackoverflow.com/questions/19113532/qgraphicsview-zooming-in-and-out-under-mouse-position-using-mouse-wheel
void InteractiveView::wheelEvent(QWheelEvent *event)
{
    //double angle = event->angleDelta().y();
    //double factor = qPow(d_ptr->mZoomFactorBase, angle);
    
    double factor = 1.25; // zoom in
    if (event->angleDelta().y() < 0) {
        factor = 1/1.25; // zoom out
    }

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
    qDebug() << "Zoom factor: " << factor;
    qDebug() << "scale: " << d_ptr->mScale;
    qDebug() << "Zoom x scale: " << transform().m11();
    qDebug() << "Zoom y scale: " << transform().m12();
    if (factor == 1) return;


    if (factor < 1 && d_ptr->mScale < 0.06) return;
    if (factor > 1 && d_ptr->mScale > 256) return;
    
    scale(factor, factor);
    centerOn(d_ptr->mTargetScenePos);

    QPointF delta_viewport_pos = d_ptr->mTargetViewportPos - QPointF(viewport()->width() / 2.0,
                                                               viewport()->height() / 2.0);
    QPointF viewport_center = mapFromScene(d_ptr->mTargetScenePos) - delta_viewport_pos;

    qDebug() << delta_viewport_pos << ", " << viewport_center;
    centerOn(mapToScene(viewport_center.toPoint()));

    d_ptr->mScale *= factor;

    emit scaleChanged(d_ptr->mScale);
}

double InteractiveView::GetScale() const {
    return d_ptr->mScale;
}

void InteractiveView::SetScale(double value) {
//    auto pos = QPoint(viewport()->rect().width()/2, viewport()->height()/2);
    
//    d_ptr->mTargetViewportPos = pos;
//    d_ptr->mTargetScenePos = mapToScene(pos);
    Zoom(value / d_ptr->mScale);

//    FitInView(d_ptr->mTargetScenePos.x()-960/2,d_ptr->mTargetScenePos.y()-720/2, 960, 720, Qt::KeepAspectRatio);

//    setTransformationAnchor(QGraphicsView::AnchorViewCenter);
//    scale(value / d_ptr->mScale, value / d_ptr->mScale);
//    d_ptr->mScale = 1;
//    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);


}

void InteractiveViewPrivate::SetRuleBarVisiable(bool value)
{
    mRuleBarVisiable = value;
    mHRuler->setVisible(value);
    mVRuler->setVisible(value);
    mBox->setVisible(value);
}
