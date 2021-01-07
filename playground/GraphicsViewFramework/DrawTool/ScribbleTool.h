#ifndef SCRIBBLETOOL_H
#define SCRIBBLETOOL_H
#include "DrawTool.h"
#include "DrawObj/GraphicsScribbleItem.h"
#include "DrawObj/GraphicsPathItem.h"

namespace gvf {

class GraphicsScribbleItem;
class GraphicsPathItem;

class ScribbleTool : public DrawTool
{
public:
    ScribbleTool(DrawShape drawShape);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent * event , GraphicsScene * scene ) ;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event , GraphicsScene * scene ) ;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * event , GraphicsScene * scene );
    //GraphicsScribbleItem *m_item;
    GraphicsPathItem * m_item;
    bool m_scribbling = false; //绘制中
};

} // namespace gvf


#endif // SCRIBBLETOOL_H
