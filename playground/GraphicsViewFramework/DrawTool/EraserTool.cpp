#include "EraserTool.h"
#include "DrawTool/DrawTool.h"
#include "../GraphicsScene.h"
#include "DrawObj/GraphicsPathItem.h"

namespace gvf {

static EraserTool eraserTool(eraser);

EraserTool::EraserTool(DrawShape drawShape)
    : DrawTool(drawShape), m_erasering(false) {
}

void EraserTool::mousePressEvent(QGraphicsSceneMouseEvent *event, GraphicsScene *scene)
{
    if ( event->button() != Qt::LeftButton ) return;
    DrawTool::mousePressEvent(event,scene);

    auto p = scene->curLayer()->mapFromScene(event->scenePos());
    if (!scene->curLayer()->boundingRect().contains(p))
        return;

    scene->clearSelection();

    // TODO 使用更细粒度的擦除
    QGraphicsItem * topitem = scene->itemAt(event->scenePos(), QTransform());
    AbstractShape * item = 0;
    if (topitem)
        item = qgraphicsitem_cast<AbstractShape*>(topitem);

    if (item)
        m_item = qgraphicsitem_cast<GraphicsPathItem*>(item);

    if (m_item) {
        //m_item->setEraser(true);
        //m_item->setEraserWidth(DrawTool::c_eraserWidth);
        //m_erasering = true;
        //m_item->setBeginPoint(event->scenePos());
        scene->removeItem(topitem);
        m_item = nullptr;
    }
}

void EraserTool::mouseMoveEvent(QGraphicsSceneMouseEvent *event, GraphicsScene *scene)
{
    if ( event->button() != Qt::LeftButton && !m_erasering) return;

    if (m_item) {
        m_item->setEndPoint(event->scenePos());
    }
}

void EraserTool::mouseReleaseEvent(QGraphicsSceneMouseEvent *event, GraphicsScene *scene)
{
    if ( event->button() != Qt::LeftButton && !m_erasering) return;

    if (m_item) {
        scene->clearSelection();
        m_item = nullptr;
    }
    m_erasering = false;
}

} // namespace gvf
