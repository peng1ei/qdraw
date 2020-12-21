#pragma once
#include <QGraphicsView>

QT_BEGIN_NAMESPACE
class QWheelEvent;
class QMouseEvent;
class QResizeEvent;
class QPaintEvent;
QT_END_NAMESPACE

class InteractiveViewPrivate;

class InteractiveView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit InteractiveView(QWidget *parent = nullptr);
    ~InteractiveView();

    double GetScale() const;
    void SetScale(double value);

    void SetRuleBarVisiable(bool value);

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
    InteractiveViewPrivate *d_ptr;
};
