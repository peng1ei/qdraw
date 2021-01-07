#include "ScribbleTool.h"
#include "SelectTool.h"
#include "../GraphicsScene.h"
#include "DrawObj/GraphicsScribbleItem.h"
#include "DrawObj/GraphicsPathItem.h"

namespace gvf {

extern SelectMode selectMode;
extern int nDragHandle;
extern SelectTool selectTool;

static ScribbleTool scribbleTool(scribble);

ScribbleTool::ScribbleTool(DrawShape drawShape)
    : DrawTool(drawShape) {
    m_item = nullptr;
}

void ScribbleTool::mousePressEvent(QGraphicsSceneMouseEvent *event, GraphicsScene *scene)
{
    if ( event->button() != Qt::LeftButton ) return;
    DrawTool::mousePressEvent(event,scene);

    auto p = scene->curLayer()->mapFromScene(event->scenePos());
    if (!scene->curLayer()->boundingRect().contains(p))
        return;
    scene->clearSelection();

    // TODO 寻找该点下的item

    switch ( c_drawShape ){
    case scribble:
        m_item = new GraphicsPathItem;
        break;
    case eraser:
        break;
    default:
        break;
    }
    if ( m_item == 0) return;

    m_scribbling = true;

    m_item->setPenColor(DrawTool::c_penColor);
    m_item->setBrushColor(DrawTool::c_brushColor);
    m_item->setPenWidth(DrawTool::c_penWidth);

    scene->addToCurLayer(m_item);
    m_item->setPos(event->scenePos());
    m_item->setSelected(true);
    m_item->setBeginPoint(event->scenePos());

    //selectMode = size;
    //nDragHandle = RightBottom;
}

void ScribbleTool::mouseMoveEvent(QGraphicsSceneMouseEvent *event, GraphicsScene *scene)
{
    if ( event->button() != Qt::LeftButton  && !m_scribbling) return;

    selectTool.mouseMoveEvent(event,scene);

    if (m_item) {
        m_item->setEndPoint(event->scenePos());
    }
}

void ScribbleTool::mouseReleaseEvent(QGraphicsSceneMouseEvent *event, GraphicsScene *scene)
{
    if ( event->button() != Qt::LeftButton && !m_scribbling) return;

    selectTool.mouseReleaseEvent(event,scene);
    if (m_item) {
        scene->clearSelection();
        m_item = nullptr;
    }

    m_scribbling = false;
    selectMode = none;
    nDragHandle = Handle_None;
}

} // namespace scribble
