#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QGraphicsScene>
#include <QGraphicsItemGroup>
#include <QGraphicsRectItem>
#include <QDebug>

int s_x = 50;
int s_y = 50;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mView = new InteractiveView;
    //setCentralWidget(mView);


    mScene = new QGraphicsScene(this);
    mScene->setSceneRect(0,0,640,480);
    mScene->setBackgroundBrush(Qt::darkGray);
    //ui->graphicsView->setScene(mScene);

    mView->setMinimumSize(640, 480);
    mView->setScene(mScene);
    //mView->setBackgroundBrush(Qt::black);
    setCentralWidget(mView);

    mLayer0 = new Layer;
    mLayer1 = new Layer(Qt::black);
    mScene->addItem(mLayer1);
    mScene->addItem(mLayer0);

    ui->checkBox_Layer0->setChecked(true);
    ui->checkBox_Layer1->setChecked(true);

    connect(ui->checkBox_Layer0, &QCheckBox::stateChanged,
            this, &MainWindow::OnLayerVisiableChanged);
    connect(ui->checkBox_Layer1, &QCheckBox::stateChanged,
            this, &MainWindow::OnLayerVisiableChanged);
    connect(ui->horizontalSlider_Layer0, &QSlider::valueChanged,
            this, &MainWindow::OnOpacityChanged);
    connect(ui->horizontalSlider_Layer1, &QSlider::valueChanged,
            this, &MainWindow::OnOpacityChanged);

    connect(ui->pushButton_AddLayer, &QPushButton::clicked,
            this, &MainWindow::OnAddLayer);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::OnAddLayer()
{
    QGraphicsRectItem *rect = new QGraphicsRectItem(s_x+=5, s_y+=5, 96, 72);
    if (s_x >= 255) {
        //s_x = 255;
        //s_y = 255;
        rect->setBrush(Qt::red);
    } else {
        rect->setBrush(QColor(s_x, s_y, 128));
    }

    //rect->setFlag(QGraphicsItem::GraphicsItemFlag::ItemIsMovable, true);
    rect->setFlags(QGraphicsItem::GraphicsItemFlag::ItemIsSelectable
                   | QGraphicsItem::GraphicsItemFlag::ItemIsMovable);
    //mLayer1->setFlags(QGraphicsItem::GraphicsItemFlag::ItemIsSelectable | QGraphicsItem::GraphicsItemFlag::ItemIsMovable);
    mLayer1->setHandlesChildEvents(false);
    qDebug() << s_x << ", " << s_y;
    mLayer1->AddItem(rect);
}

void MainWindow::OnLayerVisiableChanged(int status)
{
    if (sender() == ui->checkBox_Layer0) {
        if (status == Qt::Checked)
            SetLayerVisiable(mLayer0, true);
        else if (status == Qt::Unchecked) {
            SetLayerVisiable(mLayer0, false);
        }
    } else if (sender() == ui->checkBox_Layer1) {
        if (status == Qt::Checked)
            SetLayerVisiable(mLayer1, true);
        else if (status == Qt::Unchecked) {
            SetLayerVisiable(mLayer1, false);
        }
    }
}

void MainWindow::OnOpacityChanged(int value)
{
    if (sender() == ui->horizontalSlider_Layer0) {
        mLayer0->setOpacity(value / 100.0);
    } else if (sender() == ui->horizontalSlider_Layer1) {
        mLayer1->setOpacity(value / 100.0);
    }
}

void MainWindow::SetLayerVisiable(Layer *layer, bool visiable)
{
    layer->setVisible(visiable);
}

Layer::Layer(const QColor &color, QGraphicsItem *parent)
    : QGraphicsItemGroup(parent) {

    mRectItem = new QGraphicsRectItem(0, 0, 640, 480);
    mRectItem->setBrush(color);
    addToGroup(mRectItem);

    // 各个child item 处理自己的事件
    setHandlesChildEvents(false);

    // QGraphicsItem::ItemClipsChildrenToShape 隐藏子Item超出部分
    setFlags(QGraphicsItem::ItemClipsChildrenToShape);
}

Layer::~Layer()
{

}

void Layer::AddItem(QGraphicsItem *item)
{
    addToGroup(item);
    return;
    auto childItems = this->childItems();
    for (auto &item : childItems) {
        qDebug() << item << " Z: " <<  item->zValue();
    }
}
