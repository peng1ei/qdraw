#pragma once
#include "DrawTool.h"

namespace gvf {

class RotationTool : public DrawTool
{
public:
    RotationTool();
    virtual void mousePressEvent(QGraphicsSceneMouseEvent * event , GraphicsScene * scene ) ;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event , GraphicsScene * scene ) ;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * event , GraphicsScene * scene );
    qreal lastAngle;
    QGraphicsPathItem * dashRect;
};

} // namespace gvf
