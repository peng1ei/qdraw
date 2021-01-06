#include "ScribbleTool.h"
#include "../GraphicsScene.h"
#include "DrawObj/GraphicsScribbleItem.h"

namespace gvf {

static ScribbleTool scribbleTool(scribble);

ScribbleTool::ScribbleTool(DrawShape drawShape)
    : DrawTool(scribble) {
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

    switch ( c_drawShape ){
    case scribble:
        //_path = new QPainterPath;
        //_path->moveTo(event->scenePos());
        //item = new QGraphicsPathItem(*_path,nullptr);
        //item->setFlag(QGraphicsItem::ItemIsMovable, true);
        //item->setFlag(QGraphicsItem::ItemIsSelectable, true);
        m_item = new GraphicsScribbleItem(scene->curLayer()->boundingRect().width(),
                                          scene->curLayer()->boundingRect().height());
        break;
    default:
        break;
    }
    if ( m_item == 0) return;

    m_scribbling = true;

    m_item->setPenColor(DrawTool::c_penColor);
    m_item->setBrushColor(DrawTool::c_brushColor);
    m_item->setPos( 0,0 );
    scene->addToCurLayer(m_item);
    m_item->setSelected(true);
    //m_item->setBeginPoint(event->scenePos());
    //selectMode = size;
    //nDragHandle = RightBottom;

    //QPen pen_bak=item->pen();
    //pen_bak.setWidth(9);
    //pen_bak.setColor(_mColor);
    //item->setPen(pen_bak);
}

void ScribbleTool::mouseMoveEvent(QGraphicsSceneMouseEvent *event, GraphicsScene *scene)
{
    if ( event->button() != Qt::LeftButton  && !m_scribbling) return;
    if (m_item) {
        auto p = scene->curLayer()->mapFromScene(event->scenePos());
        m_item->setEndPoint(p);
        m_item->update();
        scene->update();
    }
}

void ScribbleTool::mouseReleaseEvent(QGraphicsSceneMouseEvent *event, GraphicsScene *scene)
{
    if ( event->button() != Qt::LeftButton && !m_scribbling) return;
    if (m_item) {
        m_item->setBeginPoint(QPointF());
        m_item->update();
        scene->clearSelection();
        m_item = nullptr;
    }

    m_scribbling = false;
}



} // namespace scribble
