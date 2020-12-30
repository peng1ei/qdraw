#pragma once
#include "DrawTool.h"

namespace gvf {

class RubberBandZoomTool : public DrawTool
{
public:
    RubberBandZoomTool();
    virtual void mousePressEvent(QGraphicsSceneMouseEvent * event , GraphicsScene * scene ) ;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event , GraphicsScene * scene ) ;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * event , GraphicsScene * scene );
    QPointF initialPositions;
    QPointF opposite_;
    QGraphicsPathItem * dashRect;
    GraphicsItemGroup * selLayer;
};

} // namespace gvf
