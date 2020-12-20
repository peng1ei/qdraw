#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "GraphicsView.h"
#include <QMainWindow>
#include <QGraphicsItemGroup>
#include <QGraphicsRectItem>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
class QGraphicsItemGroup;
class QGraphicsScene;
class QGraphicsRectItem;
class QGraphicsItem;
QT_END_NAMESPACE

class Layer : public QGraphicsItemGroup {
public:
    explicit Layer(const QColor &color = Qt::white, QGraphicsItem *parent = nullptr);
    ~Layer();

    void AddItem(QGraphicsItem *item);
    //void RemoveItem(QGraphicsItem *item);

private:
    QGraphicsRectItem *mRectItem;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void OnAddLayer();
    void OnLayerVisiableChanged(int status);
    void OnOpacityChanged(int value);

private:
    void SetLayerVisiable(Layer *layer, bool visiable);

private:
    Ui::MainWindow *ui;

    Layer *mLayer0;
    Layer *mLayer1;

    InteractiveView *mView;
    QGraphicsScene *mScene;
};
#endif // MAINWINDOW_H
