#pragma once
#include <QObject>
#include <QStandardItem>
#include <QList>
#include <QGraphicsItem>
#include <QMultiMap>

class LayerNode;
class QGraphicsPixmapItem;

class Node : public QStandardItem {
public:
    enum NodeType {
        Root,
        Layer,
        Leaf
    };

    explicit Node(NodeType type);
    virtual ~Node();

    NodeType GetNodeType() const;
    void SetNodeType(NodeType nodeType);

protected:
    NodeType mType;
    uint16_t mId;
};

class LeafNode : public Node {
public:
    LayerNode* GetLayerParent();

protected:
    LayerNode *mParentLayer;
};

class LayerNode : public Node {
public:
    LayerNode();
    explicit LayerNode(const QString &text);
    LayerNode(const QIcon &icon, const QString &text);
    virtual ~LayerNode();

    void AddNode(LeafNode *node);
    void AddLayer(LayerNode *layer);
    void RemoveNode(LeafNode *node);
    void RemoveLayer(LayerNode *node);

    void SetParentLayer(LayerNode *parent);
    LayerNode* GetParentLayer();

    QList<LayerNode*> GetLayers();
    QList<LeafNode*> GetNodes();

    int GetLayerCount() const;
    int GetNodeCount() const;

    QString Name() const;
    void SetName(const QString &name);

    LayerNode* GetLayer(const QString &name);
    LeafNode* GetNode(const QString &name);

protected:
    LayerNode *mParentLayer = nullptr;
    //QList<LayerNode*> mLayerChildrens;
    //QList<LeafNode*> mNodeChildrens;
    QMultiMap<QString, LayerNode*> mMultiMapLayerNodes;
    QMultiMap<QString, LeafNode*> mMultiMapLeafNodes;
};

class ImageLayer : public LayerNode {
public:

private:
    QString mImgPath;
};

class FolderLayer : public LayerNode {
public:

};

// VideoLayer -> ImageLayer -> LeafNode
class VideoLayer : public LayerNode {
public:

};

class AnnotationNode : public LeafNode {
public:

};

class NodeManager {
public:
    Node *GetNode(const QModelIndex &index);
};
