#ifndef LAYER_H
#define LAYER_H
#include <QList>
#include <QIcon>
#include <QStandardItem>
#include <QGraphicsItemGroup>

class Node;



class Layer : public QStandardItem {
public:
    Layer();
    explicit Layer(const QString &text);
    Layer(const QIcon &icon, const QString &text);
    virtual ~Layer();

    void AddNode(Node *node);
    void AddLayer(Layer *layer);
    void RemoveNode(Node *node);
    void RemoveLayer(Layer *node);

    void SetParentLayer(Layer *parent);
    Layer* GetParentLayer();

    QList<Layer*> GetLayers();
    QList<Node*> GetNodes();

    int GetLayerCount() const;
    int GetNodeCount() const;

    QString Name() const;
    void SetName(const QString &name);

    Layer* GetLayer(const QString &name);
    Node* GetNode(const QString &name);

protected:
    QGraphicsItemGroup *mItemGroup = nullptr;
    Layer *mParentLayer = nullptr;
    QList<Layer*> mLayerChildrens;
    QList<Node*> mNodeChildrens;
};

#endif // LAYER_H
