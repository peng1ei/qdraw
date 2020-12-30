#pragma once
#include "DrawTool.h"

namespace gvf {

class RectTool : public DrawTool
{
public:
    RectTool(DrawShape drawShape);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent * event , GraphicsScene * scene ) ;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event , GraphicsScene * scene ) ;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * event , GraphicsScene * scene );
    GraphicsItem * item;
};

} // namespace gvf
