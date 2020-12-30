#pragma once
#include "DrawTool.h"

namespace gvf {

class PolygonTool : public DrawTool
{
public:
    PolygonTool(DrawShape shape );
    virtual void mousePressEvent(QGraphicsSceneMouseEvent * event , GraphicsScene * scene ) ;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event , GraphicsScene * scene ) ;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * event , GraphicsScene * scene );
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event ,GraphicsScene *scene );
    GraphicsPolygonItem * item;
    int m_nPoints;
    QPointF initialPositions;
};

} // namespace gvf
