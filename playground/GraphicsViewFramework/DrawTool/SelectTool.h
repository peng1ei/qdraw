#pragma once
#include "DrawTool.h"

QT_BEGIN_NAMESPACE
class QGraphicsPathItem;
QT_END_NAMESPACE

namespace gvf {

class GraphicsItemGroup;

class SelectTool : public DrawTool
{
public:
    SelectTool();
    virtual void mousePressEvent(QGraphicsSceneMouseEvent * event , GraphicsScene * scene ) ;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event , GraphicsScene * scene ) ;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * event , GraphicsScene * scene );
    QPointF initialPositions;
    //QPointF opposite_;
    QGraphicsPathItem * dashRect;
    GraphicsItemGroup * selLayer;
};

extern SelectTool selectTool;

} // namespace gvf
