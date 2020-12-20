#pragma once
#include <QGraphicsView>

QT_BEGIN_NAMESPACE
class QWheelEvent;
class QMouseEvent;
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

protected:
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

signals:
    void scaleChanged(double value);

private:
    void Pan(QPointF delta);
    void Zoom(double factor);

private:
    InteractiveViewPrivate *d_ptr;
};
