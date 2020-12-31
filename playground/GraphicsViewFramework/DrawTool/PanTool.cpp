#include "PanTool.h"
#include "../GraphicsScene.h"

namespace gvf {

static PanTool panTool;

PanTool::PanTool()
    : DrawTool(pan) {

}

void PanTool::mousePressEvent(QGraphicsSceneMouseEvent *event, GraphicsScene *scene)
{
    Q_UNUSED(event)
    Q_UNUSED(scene)
    setCursor(scene, Qt::ClosedHandCursor);
}

void PanTool::mouseMoveEvent(QGraphicsSceneMouseEvent *event, GraphicsScene *scene)
{
    Q_UNUSED(event)
    Q_UNUSED(scene)
}

void PanTool::mouseReleaseEvent(QGraphicsSceneMouseEvent *event, GraphicsScene *scene)
{
    Q_UNUSED(event)
    Q_UNUSED(scene)
    setCursor(scene, Qt::OpenHandCursor);
}

}// namespace gvf
