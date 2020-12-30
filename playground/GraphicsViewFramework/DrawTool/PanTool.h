#pragma once
#include "DrawTool.h"

namespace gvf {

class PanTool : public DrawTool
{
public:
    PanTool();
    virtual void mousePressEvent(QGraphicsSceneMouseEvent * event , GraphicsScene * scene ) ;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event , GraphicsScene * scene ) ;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * event , GraphicsScene * scene );
};

} // namespace gvf
