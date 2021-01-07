#ifndef ERASERTOOL_H
#define ERASERTOOL_H
#include "DrawTool.h"

namespace gvf {

class GraphicsPathItem;

class EraserTool : public DrawTool
{
public:
    EraserTool(DrawShape drawShape);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent * event , GraphicsScene * scene ) ;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event , GraphicsScene * scene ) ;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * event , GraphicsScene * scene );
    GraphicsPathItem *m_item = nullptr;
    bool m_erasering = false;
};

}

#endif // ERASERTOOL_H
