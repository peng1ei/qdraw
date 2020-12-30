#pragma once
#include <QGraphicsView>

QT_BEGIN_NAMESPACE
class QWheelEvent;
class QMouseEvent;
class QResizeEvent;
class QPaintEvent;
QT_END_NAMESPACE

namespace gvf {

class GraphicsViewPrivate;

class GraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit GraphicsView(QWidget *parent = nullptr);
    ~GraphicsView();

    double GetScale() const;
    void SetScale(double value);

    void SetRuleBarVisiable(bool value);
    
    void FitInView(const QRectF &rect, Qt::AspectRatioMode aspectRadioMode = Qt::KeepAspectRatio);
    void FitInView(qreal x, qreal y, qreal w, qreal h,
                          Qt::AspectRatioMode aspectRadioMode = Qt::KeepAspectRatio);
    void FitInView(const QGraphicsItem *item, Qt::AspectRatioMode aspectRadioMode = Qt::KeepAspectRatio);

    void ZoomInUnderViewCenter();
    void ZoomOutUnderViewCenter();
    void Zoom1To1();
    void Zoom1To1(int x, int y, int w, int h);
    //void Zoom1To1(Layer *layer);
    void ZoomToRect();
    void SetPan(bool value);

signals:
    void scaleChanged(double value);
    void posFromSceneChanged(double x, double y);
    void posFromViewChanged(int x, int y);

protected:
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
    void scrollContentsBy(int dx, int dy) Q_DECL_OVERRIDE;

    void paintEvent(QPaintEvent *e) Q_DECL_OVERRIDE;

protected:
    void UpdateRuler();
    void ResizeRuler();



private:
    void Pan(QPointF delta);
    void Zoom(double factor);

private:
    GraphicsViewPrivate *d_ptr;
};

} // namespace gvf
