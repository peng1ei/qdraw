#ifndef TREEVIEW_H
#define TREEVIEW_H
#include <QTreeView>

class TreeViewPrivate;
class Layer;

class TreeView : public QTreeView {
    Q_OBJECT
public:
    void AddLayer(Layer *layer);
    Layer* GetLayer();

protected:
    TreeViewPrivate *d_ptr;
};


#endif // TREEVIEW_H
