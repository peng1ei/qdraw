#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "ObjectController.h"
#include "GraphicsScene.h"
#include "GraphicsView.h"
#include "Utils/ImageFileListProviderThd.h"
#include "Utils/ProgressBarDialog.h"
#include "Utils/ImageFileListModel.h"

#include <QGraphicsScene>
#include <QGraphicsItemGroup>
#include <QGraphicsRectItem>
#include <QPen>
#include <QDockWidget>
#include <QGraphicsItem>
#include <QMdiSubWindow>
#include <QUndoStack>
#include <QUndoView>
#include <QToolBar>
#include <QApplication>
#include <QLabel>
#include <QComboBox>
#include <QFileDialog>
#include <QFileInfo>
#include <QImageReader>
#include <QListView>
#include <QStringListModel>
#include <QMessageBox>
#include <QGLWidget>
#include <QDebug>
#include <QColorDialog>

int s_x = 50;
int s_y = 50;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

#if 0
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

    
#endif
    
    InitUI();
    mTheControlledObject = nullptr;
    
    
#if 1
    mLayer0 = new Layer;
    mLayer1 = new Layer(Qt::darkGreen);
    //mScene->addItem(mLayer1);
    //mScene->addItem(mLayer0);

    ui->checkBox_Layer0->setChecked(true);
    ui->checkBox_Layer1->setChecked(true);

    connect(ui->checkBox_Layer0, &QCheckBox::stateChanged,
            this, &MainWindow::OnLayerVisiableChanged);
    connect(ui->checkBox_Layer1, &QCheckBox::stateChanged,
            this, &MainWindow::OnLayerVisiableChanged);
    connect(ui->checkBox_RuleBar, &QCheckBox::stateChanged,
            this, &MainWindow::OnLayerVisiableChanged);
    connect(ui->horizontalSlider_Layer0, &QSlider::valueChanged,
            this, &MainWindow::OnOpacityChanged);
    connect(ui->horizontalSlider_Layer1, &QSlider::valueChanged,
            this, &MainWindow::OnOpacityChanged);

    connect(ui->pushButton_AddLayer, &QPushButton::clicked,
            this, &MainWindow::OnAddLayer);
    
//    auto viewRect = mView->viewport()->rect();
//    auto viewCenterPoint = viewRect.center();
//    auto sceneCenterPoint = mView->mapToScene(viewCenterPoint);
//    qDebug() << "0,0 map to scene: " << mView->mapToScene(viewCenterPoint);
//    qDebug() << "resize view Rect: " << viewRect;
//    qDebug() << "resize view center: " << viewCenterPoint;
    
//    mScene->addRect(sceneCenterPoint.x()-320,sceneCenterPoint.y()-240,640, 480);
#endif 

    setWindowIcon(QIcon(":/image/icons/via.ico"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    
    //auto viewRect = mView->viewport()->rect();
    //auto viewCenterPoint = viewRect.center();
    
//    auto sceneCenterPoint = mView->mapToScene(viewCenterPoint);
//    qDebug() << "0,0 map to scene: " << mView->mapToScene(viewCenterPoint);
//    qDebug() << "resize view Rect: " << viewRect;
//    qDebug() << "resize view center: " << viewCenterPoint;
    
    //mScene->addRect(sceneCenterPoint.x()-320,sceneCenterPoint.y()-240,640, 480);
}

bool MainWindow::eventFilter(QObject *obj, QEvent *evt)
{
    // TODO
    if(obj == mView  &&  evt->type() == QEvent::ContextMenu)
    {
        //qDebug() << "item count: " << mScene->items().count();
        if(!mScene->items().empty()) {
              mContextMenu->exec(cursor().pos()); //在当前鼠标位置上运行菜单menu对象
              //mContextMenu->setActiveAction(mUiSelectAct);
              //mContextMenu->setFocus();
        }
    }

    return QWidget::eventFilter(obj,evt);
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
    } else if (sender() == ui->checkBox_RuleBar) {
        if (status == Qt::Checked)
            mView->SetRuleBarVisiable(false);
        else if (status == Qt::Unchecked) {
            mView->SetRuleBarVisiable(true);
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

void MainWindow::OnAddShape()
{
    // TODO 当绘图工具发生变化时，应对外发出工具改变信号，并传出当前的工具类型
    if ( sender() == mUiSelectAct )
        DrawTool::c_drawShape = selection;
    else if (sender() == mUiLineAct )
        DrawTool::c_drawShape = line;
    else if ( sender() == mUiRectAct )
        DrawTool::c_drawShape = rectangle;
    else if ( sender() == mUiRoundRectAct )
        DrawTool::c_drawShape = roundrect;
    else if ( sender() == mUiEllipseAct )
        DrawTool::c_drawShape = ellipse ;
    else if ( sender() == mUiPolygonAct )
        DrawTool::c_drawShape = polygon;
    else if ( sender() == mUiBezierAct )
        DrawTool::c_drawShape = bezier ;
    else if (sender() == mUiRotateAct )
        DrawTool::c_drawShape = rotation;
    else if (sender() == mUiPolylineAct )
        DrawTool::c_drawShape = polyline;

    if ( sender() != mUiSelectAct && sender() != mUiRotateAct ){
        mScene->clearSelection();
    }

    if (sender() == mUiSelectAct || sender() == mUiRotateAct) {
        mView->setCursor(Qt::ArrowCursor);
    }

    //mView->SetPan(false);
}

void MainWindow::OnSelectColor()
{
    QColor color=QColorDialog::getColor(DrawTool::c_brushColor,this);
    if(!color.isValid())
        return;

    DrawTool::c_penColor = color;
    DrawTool::c_brushColor = color;
}

void MainWindow::OnNewFile()
{
    auto viewRect = mView->viewport()->rect();
    auto viewCenterPoint = viewRect.center();
    //auto sceneCenterPoint = mView->mapToScene(viewCenterPoint);
    qDebug() << "0,0 map to scene: " << mView->mapToScene(viewCenterPoint);
    qDebug() << "resize view Rect: " << viewRect;
    qDebug() << "resize view center: " << viewCenterPoint;
    
    int w = 960, h = 720;
    //Layer *layer = new Layer(sceneCenterPoint.x()-w/2,sceneCenterPoint.y()-h/2, w, h);
    Layer *layer = new Layer(0,0, w, h);
    mScene->addItem(layer);
    //layer->setX(0);
    //layer->setY(0);
    qDebug() << "x scale: " << mView->transform().m11();
    qDebug() << "y scale: " << mView->transform().m12();
    mView->Zoom1To1();
    qDebug() << "x scale: " << mView->transform().m11();
    qDebug() << "y scale: " << mView->transform().m12();

    qDebug() << "Layer Rect: " << layer->GetRectFromScene();
}

void MainWindow::OnOpen()
{
    QString filePath = QFileDialog::getOpenFileName(this,
                                                    tr("Open image"),
                                                    QString("D:/Data/default"),
                                                    "Image File(*.png *.jpg *.jpeg *.bmp *.ico *.tif *.gif *.svg)");
    if (filePath.isEmpty()) {
        return;
    }

    UpdateImageFileListModel(QStringList(filePath));
    UpdateScene(filePath);
}

void MainWindow::OnOpenDir()
{
    QString dirPath = QFileDialog::getExistingDirectory(this,
                                                    tr("Open Dir"),
                                                    tr("D:/Data"));
    if (dirPath.isEmpty()) {
        return;
    }

    // TODO 放到独立线程去做
    QStringList filters = { "*.png", "*.jpg", "*.jpeg", "*.bmp", "*.ico", "*.gif", "*.svg" };
    mImageFileListProviderThd = new ImageFileListProviderThd(dirPath, filters);
    connect(mImageFileListProviderThd, &ImageFileListProviderThd::finished,
            this, &MainWindow::OnOpenDirFinished);
    connect(mImageFileListProviderThd, &ImageFileListProviderThd::sigResult,
            this, &MainWindow::OnOpenDirResult);

    mImageFileListProviderThd->SetListMode(mImageFileListModel);
    mImageFileListProviderThd->start();

    mProgressBarDlg = new ProgressBarDialog(ProgressBarDialog::Busy, this);
    mProgressBarDlg->SetText(tr("Reading folder..."));
    mProgressBarDlg->exec();
}

void MainWindow::OnSave()
{

}

void MainWindow::OnAbout()
{
    
}

void MainWindow::OnFuncTestTriggered()
{
    
}

void MainWindow::OnBringToFrontTriggered()
{
    
}

void MainWindow::OnSendToBackTriggered()
{
    
}

void MainWindow::OnAglinTriggered()
{
    
}

void MainWindow::OnGroupTriggered()
{
    
}

void MainWindow::OnUnGroupTriggered()
{
    
}

void MainWindow::OnCopy()
{
    
}

void MainWindow::OnPaste()
{
    
}

void MainWindow::OnCut()
{
    
}

void MainWindow::OnZoomIn()
{
    mView->ZoomInUnderViewCenter();
}

void MainWindow::OnZoomOut()
{
    mView->ZoomOutUnderViewCenter();
}

void MainWindow::OnZoomFitView()
{
    auto rect = mScene->itemsBoundingRect();
    qDebug() << rect;

    mView->FitInView(rect, Qt::KeepAspectRatio);
}

void MainWindow::OnZoomOne()
{
    mView->Zoom1To1();
}

void MainWindow::OnZoomToRect()
{
    DrawTool::c_drawShape = rubberbandzoom;
    //mView->ZoomToRect(); // TODO
    //mView->SetPan(false);
    mView->setCursor(QCursor(QPixmap(":/image/icons/zoomrectcur.png")));
}

void MainWindow::OnPan()
{
    DrawTool::c_drawShape = pan;
    //mView->SetPan(true);
    mView->setCursor(Qt::ClosedHandCursor);
}

void MainWindow::OnDeleteItem()
{
    
}

void MainWindow::OnPosFromSceneChanged(double x, double y)
{
    mUiMousePosFromSceneInfo->setText(QString(" scene: %1, %2 ")
                                      .arg(QString::number(x,'f',2)).arg(QString::number(y,'f',2)));
}

void MainWindow::OnPosFromViewChanged(int x, int y)
{
    mUiMousePosFromViewInfo->setText(QString(" view: %1, %2 ")
                                     .arg(QString::number(x)).arg(QString::number(y)));
}

void MainWindow::OnOpenDirFinished()
{
    // TODO hide, not delete
    delete mProgressBarDlg;
    mProgressBarDlg = nullptr;

    // TODO move OpenDirManager
    delete mImageFileListProviderThd;
    mImageFileListProviderThd = nullptr;

    if (mImageFileListModel->rowCount() > 0) {
        mImageFileListModel->data(mImageFileListModel->index(0)).toString();
        UpdateScene(mImageFileListModel->data(mImageFileListModel->index(0)).toString());
    }
}

void MainWindow::OnOpenDirResult(bool result, QString msg)
{
    if (result == false) {
        QMessageBox::warning(this, tr("Warning"), msg);
        return;
    }
}

void MainWindow::OnImageListViewDoubleClicked(const QModelIndex &index)
{
    if (!index.isValid())
        return;

    UpdateScene(mImageFileListModel->data(index).toString());

    // TODO 自动保存，点击下一张或上一张时自动保存标注结果
    mImageFileListModel->setData(index, Qt::Checked, Qt::CheckStateRole);
}

void MainWindow::OnItemSelected()
{
    QGraphicsScene * scene = mScene;

    if ( scene->selectedItems().count() > 0
         && scene->selectedItems().first()->isSelected())
    {
        QGraphicsItem *item = scene->selectedItems().first();

        mTheControlledObject = dynamic_cast<QObject*>(item);
        mPropertyEditor->setObject(mTheControlledObject);
    }
    return ;
    if ( mTheControlledObject )
    {
        mPropertyEditor->setObject(mTheControlledObject);
    }
}

void MainWindow::OnItemMoved(QGraphicsItem *item, const QPointF &oldPosition)
{
    
}

void MainWindow::OnItemRotate(QGraphicsItem *item, const qreal oldAngle)
{
    
}

void MainWindow::OnItemAdded(QGraphicsItem *item)
{
    
}

void MainWindow::OnItemResize(QGraphicsItem *item, int handle, const QPointF &scale)
{
    
}

void MainWindow::OnItemControl(QGraphicsItem *item, int handle, const QPointF &newPos, const QPointF &lastPos_)
{
    
}

void MainWindow::OnScaleChanged(double scale)
{
    mUiScaleInfo->setText(QString(" scale: %1\% ").arg(QString::number(scale*100,'f',0)));
}

void MainWindow::SetLayerVisiable(Layer *layer, bool visiable)
{
    layer->setVisible(visiable);
}

void MainWindow::InitUI()
{
    // Undo and Redo
    mUiUndoStack = new QUndoStack(this);
    mUiUndoView = new QUndoView(mUiUndoStack);
    mUiUndoView->setWindowTitle(tr("Command List"));
    mUiUndoView->setAttribute(Qt::WA_QuitOnClose, false);
    QDockWidget *dock = new QDockWidget(this);
    addDockWidget(Qt::RightDockWidgetArea, dock);
    dock->setWidget(mUiUndoView);
    
    InitGraphicsView();

    CreateActions();
    CreateMenus();
    CreateToolbars();
    CreateToolBox();
    CreatePropertyEditor();

    CreateStatusBar();

    CreateImageListView();
}

void MainWindow::CreateActions()
{
    mUiNewAct = new QAction(tr("&New"), this);
    mUiNewAct->setShortcuts(QKeySequence::New);
    mUiNewAct->setStatusTip(tr("Create a new file"));
    connect(mUiNewAct, SIGNAL(triggered()), this, SLOT(OnNewFile()));

    mUiOpenAct = new QAction(tr("&Open..."), this);
    mUiOpenAct->setShortcuts(QKeySequence::Open);
    mUiOpenAct->setStatusTip(tr("Open an existing file"));
    connect(mUiOpenAct, SIGNAL(triggered()), this, SLOT(OnOpen()));

    mUiOpenDirAct = new QAction(tr("&Open Dir..."), this);
    mUiOpenDirAct->setShortcuts(QKeySequence::Open);
    mUiOpenDirAct->setStatusTip(tr("Open an existing dir"));
    connect(mUiOpenDirAct, SIGNAL(triggered()), this, SLOT(OnOpenDir()));

    mUiSaveAct = new QAction(tr("&Save"), this);
    mUiSaveAct->setShortcuts(QKeySequence::Save);
    mUiSaveAct->setStatusTip(tr("Save the document to disk"));
    connect(mUiSaveAct, SIGNAL(triggered()), this, SLOT(OnSave()));

    mUiExitAct = new QAction(tr("E&xit"), this);
    mUiExitAct->setShortcuts(QKeySequence::Quit);
    mUiExitAct->setStatusTip(tr("Exit the application"));
    connect(mUiExitAct, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));

    // TODO 控制打开的文档
//    mUiCloseAct = new QAction(tr("Cl&ose"), this);
//    mUiCloseAct->setStatusTip(tr("Close the active window"));
//    connect(mUiCloseAct, SIGNAL(triggered()),
//            this, SLOT(CloseWindow()));

//    mUiCloseAllAct = new QAction(tr("Close &All"), this);
//    closeAllAct->setStatusTip(tr("Close all the windows"));
//    connect(closeAllAct, SIGNAL(triggered()),
//            mdiArea, SLOT(closeAllSubWindows()));

//    mUiTileAct = new QAction(tr("&Tile"), this);
//    tileAct->setStatusTip(tr("Tile the windows"));
//    connect(tileAct, SIGNAL(triggered()), mdiArea, SLOT(tileSubWindows()));

//    cascadeAct = new QAction(tr("&Cascade"), this);
//    cascadeAct->setStatusTip(tr("Cascade the windows"));
//    connect(cascadeAct, SIGNAL(triggered()), mdiArea, SLOT(cascadeSubWindows()));

//    nextAct = new QAction(tr("Ne&xt"), this);
//    nextAct->setShortcuts(QKeySequence::NextChild);
//    nextAct->setStatusTip(tr("Move the focus to the next window"));
//    connect(nextAct, SIGNAL(triggered()),
//            mdiArea, SLOT(activateNextSubWindow()));

//    previousAct = new QAction(tr("Pre&vious"), this);
//    previousAct->setShortcuts(QKeySequence::PreviousChild);
//    previousAct->setStatusTip(tr("Move the focus to the previous "
//                                 "window"));
//    connect(previousAct, SIGNAL(triggered()),
//            mdiArea, SLOT(activatePreviousSubWindow()));

    mUiSeparatorAct = new QAction(this);
    mUiSeparatorAct->setSeparator(true);

    mUiAboutAct = new QAction(tr("&About"), this);
    mUiAboutAct->setStatusTip(tr("Show the application's About box"));
    connect(mUiAboutAct, SIGNAL(triggered()), this, SLOT(OnAbout()));

    mUiAboutQtAct = new QAction(tr("About &Qt"), this);
    mUiAboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
    connect(mUiAboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    // create align actions
    mUiRightAct   = new QAction(QIcon(":/image/icons/align_right.png"),tr("right"),this);
    mUiLeftAct    = new QAction(QIcon(":/image/icons/align_left.png"),tr("left"),this);
    mUiVCenterAct = new QAction(QIcon(":/image/icons/align_vcenter.png"),tr("vcenter"),this);
    mUiHCenterAct = new QAction(QIcon(":/image/icons/align_hcenter.png"),tr("hcenter"),this);
    mUiUpAct      = new QAction(QIcon(":/image/icons/align_top.png"),tr("top"),this);
    mUiDownAct    = new QAction(QIcon(":/image/icons/align_bottom.png"),tr("bottom"),this);
    mUiHorzAct    = new QAction(QIcon(":/image/icons/align_horzeven.png"),tr("Horizontal"),this);
    mUiVertAct    = new QAction(QIcon(":/image/icons/align_verteven.png"),tr("vertical"),this);
    mUiHeightAct  = new QAction(QIcon(":/image/icons/align_height.png"),tr("height"),this);
    mUiWidthAct   = new QAction(QIcon(":/image/icons/align_width.png"),tr("width"),this);
    mUiAllAct     = new QAction(QIcon(":/image/icons/align_all.png"),tr("width and height"),this);

    mUiBringToFrontAct = new QAction(QIcon(":/image/icons/bringtofront.png"),tr("bring to front"),this);
    mUiSendToBackAct   = new QAction(QIcon(":/image/icons/sendtoback.png"),tr("send to back"),this);
    mUiGroupAct        = new QAction(QIcon(":/image/icons/group.png"),tr("group"),this);
    mUiUnGroupAct        = new QAction(QIcon(":/image/icons/ungroup.png"),tr("ungroup"),this);

    connect(mUiBringToFrontAct,SIGNAL(triggered()),this,SLOT(OnBringToFrontTriggered()));
    connect(mUiSendToBackAct,SIGNAL(triggered()),this,SLOT(OnSendToBackTriggered()));
    connect(mUiRightAct,SIGNAL(triggered()),this,SLOT(OnAglinTriggered()));
    connect(mUiLeftAct,SIGNAL(triggered()),this,SLOT(OnAglinTriggered()));
    connect(mUiVCenterAct,SIGNAL(triggered()),this,SLOT(OnAglinTriggered()));
    connect(mUiHCenterAct,SIGNAL(triggered()),this,SLOT(OnAglinTriggered()));
    connect(mUiUpAct,SIGNAL(triggered()),this,SLOT(OnAglinTriggered()));
    connect(mUiDownAct,SIGNAL(triggered()),this,SLOT(OnAglinTriggered()));

    connect(mUiHorzAct,SIGNAL(triggered()),this,SLOT(OnAglinTriggered()));
    connect(mUiVertAct,SIGNAL(triggered()),this,SLOT(OnAglinTriggered()));
    connect(mUiHeightAct,SIGNAL(triggered()),this,SLOT(OnAglinTriggered()));
    connect(mUiWidthAct,SIGNAL(triggered()),this,SLOT(OnAglinTriggered()));
    connect(mUiAllAct,SIGNAL(triggered()),this,SLOT(OnAglinTriggered()));

    connect(mUiGroupAct,SIGNAL(triggered()),this,SLOT(OnGroupTriggered()));
    connect(mUiUnGroupAct,SIGNAL(triggered()),this,SLOT(OnUnGroupTriggered()));

    //create draw actions
    mUiSelectAct = new QAction(QIcon(":/image/icons/arrow.png"),tr("select tool"),this);

    mUiSelectAct->setCheckable(true);
    mUiLineAct = new QAction(QIcon(":/image/icons/line.png"),tr("line tool"),this);
    mUiLineAct->setCheckable(true);
    mUiRectAct = new QAction(QIcon(":/image/icons/rectangle.png"),tr("rect tool"),this);
    mUiRectAct->setCheckable(true);

    mUiRoundRectAct =  new QAction(QIcon(":/image/icons/roundrect.png"),tr("roundrect tool"),this);
    mUiRoundRectAct->setCheckable(true);
    mUiEllipseAct = new QAction(QIcon(":/image/icons/ellipse.png"),tr("ellipse tool"),this);
    mUiEllipseAct->setCheckable(true);
    mUiPolygonAct = new QAction(QIcon(":/image/icons/polygon.png"),tr("polygon tool"),this);
    mUiPolygonAct->setCheckable(true);
    mUiPolylineAct = new QAction(QIcon(":/image/icons/polyline.png"),tr("polyline tool"),this);
    mUiPolylineAct->setCheckable(true);
    mUiBezierAct= new QAction(QIcon(":/image/icons/bezier.png"),tr("bezier tool"),this);
    mUiBezierAct->setCheckable(true);

    mUiRotateAct = new QAction(QIcon(":/image/icons/rotate.png"),tr("rotate tool"),this);
    mUiRotateAct->setCheckable(true);

    mUiSelectColorAct = new QAction(QIcon(":/image/icons/color.png"),tr("select color tool"),this);

    mUiDrawActionGroup = new QActionGroup(this);
    mUiDrawActionGroup->addAction(mUiSelectAct);

    mUiDrawActionGroup->addAction(mUiRectAct);
    mUiDrawActionGroup->addAction(mUiRoundRectAct);
    mUiDrawActionGroup->addAction(mUiEllipseAct);
    mUiDrawActionGroup->addAction(mUiLineAct);
    mUiDrawActionGroup->addAction(mUiPolylineAct);
    mUiDrawActionGroup->addAction(mUiPolygonAct);

    //mUiDrawActionGroup->addAction(mUiBezierAct);
    mUiDrawActionGroup->addAction(mUiRotateAct);

    mUiZoomToRectAct = new QAction(QIcon(":/image/icons/zoomrect.png"),tr("zoomToRect"),this);
    mUiZoomToRectAct->setCheckable(true);
    mUiDrawActionGroup->addAction(mUiZoomToRectAct);

    mUiPanAct = new QAction(QIcon(":/image/icons/pan.png"),tr("pan"),this);
    mUiPanAct->setCheckable(true);
    mUiDrawActionGroup->addAction(mUiPanAct);

    mUiSelectAct->setChecked(true);

    connect(mUiSelectAct,SIGNAL(triggered()),this,SLOT(OnAddShape()));
    connect(mUiLineAct,SIGNAL(triggered()),this,SLOT(OnAddShape()));
    connect(mUiRectAct,SIGNAL(triggered()),this,SLOT(OnAddShape()));
    connect(mUiRoundRectAct,SIGNAL(triggered()),this,SLOT(OnAddShape()));
    connect(mUiEllipseAct,SIGNAL(triggered()),this,SLOT(OnAddShape()));
    connect(mUiPolygonAct,SIGNAL(triggered()),this,SLOT(OnAddShape()));
    connect(mUiPolylineAct,SIGNAL(triggered()),this,SLOT(OnAddShape()));
    connect(mUiBezierAct,SIGNAL(triggered()),this,SLOT(OnAddShape()));
    connect(mUiRotateAct,SIGNAL(triggered()),this,SLOT(OnAddShape()));
    connect(mUiSelectColorAct,SIGNAL(triggered()),this,SLOT(OnSelectColor()));

    mUiDeleteAct = new QAction(tr("&Delete"), this);
    mUiDeleteAct->setShortcut(QKeySequence::Delete);

    mUiUndoAct = mUiUndoStack->createUndoAction(this,tr("undo"));
    mUiUndoAct->setIcon(QIcon(":/image/icons/undo.png"));
    mUiUndoAct->setShortcuts(QKeySequence::Undo);

    mUiRedoAct = mUiUndoStack->createRedoAction(this,tr("redo"));
    mUiRedoAct->setIcon(QIcon(":/image/icons/redo.png"));
    mUiRedoAct->setShortcuts(QKeySequence::Redo);

    //mUiPanAct = new QAction(QIcon(":/image/icons/pan.png"),tr("zoomIn"),this);
    mUiZoomInAct = new QAction(QIcon(":/image/icons/zoomin.png"),tr("zoomIn"),this);
    mUiZoomOutAct = new QAction(QIcon(":/image/icons/zoomout.png"),tr("zoomOut"),this);
    mUiZoomFitAct = new QAction(QIcon(":/image/icons/zoom_fit.png"),tr("zoomFitView"),this);
    mUiZoomOneAct = new QAction(QIcon(":/image/icons/zoom1-1.png"),tr("zoom1-1"),this);
    //mUiZoomToRectAct = new QAction(QIcon(":/image/icons/zoomrect.png"),tr("zoomToRect"),this);
    //mUiZoomToRectAct->setCheckable(true);

    mUiCopyAct = new QAction(QIcon(":/image/icons/copy.png"),tr("copy"),this);
    mUiCopyAct->setShortcut(QKeySequence::Copy);

    mUiPasteAct = new QAction(QIcon(":/image/icons/paste.png"),tr("paste"),this);
    mUiPasteAct->setShortcut(QKeySequence::Paste);
    mUiPasteAct->setEnabled(false);
    mUiCutAct = new QAction(QIcon(":/image/icons/cut.png"),tr("cut"),this);
    mUiCutAct->setShortcut(QKeySequence::Cut);

    connect(mUiCopyAct,SIGNAL(triggered()),this,SLOT(OnCopy()));
    connect(mUiPasteAct,SIGNAL(triggered()),this,SLOT(OnPaste()));
    connect(mUiCutAct,SIGNAL(triggered()),this,SLOT(OnCut()));

    connect(mUiZoomInAct , SIGNAL(triggered()),this,SLOT(OnZoomIn()));
    connect(mUiZoomOutAct , SIGNAL(triggered()),this,SLOT(OnZoomOut()));
    connect(mUiZoomFitAct , SIGNAL(triggered()),this,SLOT(OnZoomFitView()));
    connect(mUiZoomOneAct , SIGNAL(triggered()),this,SLOT(OnZoomOne()));
    connect(mUiZoomToRectAct , SIGNAL(triggered()),this,SLOT(OnZoomToRect()));
    connect(mUiPanAct , SIGNAL(triggered()),this,SLOT(OnPan()));
    connect(mUiDeleteAct, SIGNAL(triggered()), this, SLOT(OnDeleteItem()));

    mUiFuncAct = new QAction(tr("func test"),this);
    connect(mUiFuncAct,SIGNAL(triggered()),this,SLOT(OnFuncTestTriggered()));
}

void MainWindow::CreateMenus()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    //fileMenu->addAction(mUiNewAct);
    fileMenu->addAction(mUiOpenAct);
    fileMenu->addAction(mUiOpenDirAct);
    fileMenu->addAction(mUiSaveAct);
    fileMenu->addSeparator();
    fileMenu->addAction(mUiExitAct);

    QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(mUiUndoAct);
    editMenu->addAction(mUiRedoAct);
    editMenu->addAction(mUiCutAct);
    editMenu->addAction(mUiCopyAct);
    editMenu->addAction(mUiPasteAct);
    editMenu->addAction(mUiDeleteAct);

    QMenu *viewMenu = menuBar()->addMenu(tr("&View"));
    viewMenu->addAction(mUiPanAct);
    viewMenu->addAction(mUiZoomInAct);
    viewMenu->addAction(mUiZoomOutAct);
    viewMenu->addAction(mUiZoomFitAct);
    viewMenu->addAction(mUiZoomOneAct);
    viewMenu->addAction(mUiZoomToRectAct);

    QMenu *toolMenu = menuBar()->addMenu(tr("&Tools"));
    QMenu *shapeTool = new QMenu("&Shape");
    shapeTool->addAction(mUiSelectAct);
    shapeTool->addAction(mUiRectAct);
    //shapeTool->addAction(mUiRoundRectAct);
    shapeTool->addAction(mUiEllipseAct);
    shapeTool->addAction(mUiLineAct);
    shapeTool->addAction(mUiPolylineAct);
    shapeTool->addAction(mUiPolygonAct);
    //shapeTool->addAction(mUiBezierAct);
    shapeTool->addAction(mUiRotateAct);
    shapeTool->addAction(mUiSelectAct);
    toolMenu->addMenu(shapeTool);

#if 0
    QMenu *alignMenu = new QMenu("Align");
    alignMenu->addAction(mUiRightAct);
    alignMenu->addAction(mUiLeftAct);
    alignMenu->addAction(mUiHCenterAct);
    alignMenu->addAction(mUiVCenterAct);
    alignMenu->addAction(mUiUpAct);
    alignMenu->addAction(mUiDownAct);
    alignMenu->addAction(mUiHorzAct);
    alignMenu->addAction(mUiVertAct);
    alignMenu->addAction(mUiHeightAct);
    alignMenu->addAction(mUiWidthAct);
    alignMenu->addAction(mUiAllAct);
    toolMenu->addMenu(alignMenu);
#endif

    // TODO 控制打开的文档
    //mUiMenu = menuBar()->addMenu(tr("&Window"));
    //updateWindowMenu();// TODO
    //connect(mUiMenu, SIGNAL(aboutToShow()), this, SLOT(updateWindowMenu()));

    menuBar()->addSeparator();

    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(mUiAboutAct);
    helpMenu->addAction(mUiAboutQtAct);
    helpMenu->addAction(mUiFuncAct);

    // 右键菜单
    mContextMenu = new QMenu;
    mView->installEventFilter(this);
    //QAction *a1 = new QAction("aaaa");
    //mContextMenu->addAction(a1);
//    mView->setContextMenuPolicy(Qt::CustomContextMenu);
//    connect(mView,&InteractiveView::customContextMenuRequested,[=](const QPoint &pos)
//    {
//        //qDebug()<<pos;//参数pos用来传递右键点击时的鼠标的坐标，这个坐标一般是相对于控件左上角而言的
//        mContextMenu->exec(QCursor::pos());
//        mContextMenu->setActiveAction(mUiPanAct);
//        //mContextMenu->setFocus(Qt::FocusReason::PopupFocusReason);
//    });
    mContextMenu->addAction(mUiSelectAct);
    mContextMenu->addAction(mUiRectAct);
    //mContextMenu->addAction(mUiRoundRectAct);
    mContextMenu->addAction(mUiEllipseAct);
    mContextMenu->addAction(mUiLineAct);
    mContextMenu->addAction(mUiPolylineAct);
    mContextMenu->addAction(mUiPolygonAct);

    //mContextMenu->addAction(mUiBezierAct);
    mContextMenu->addAction(mUiRotateAct);

    mContextMenu->addSeparator();

    mContextMenu->addAction(mUiPanAct);
    mContextMenu->addAction(mUiZoomInAct);
    mContextMenu->addAction(mUiZoomOutAct);
    mContextMenu->addAction(mUiZoomFitAct);
    mContextMenu->addAction(mUiZoomOneAct);
    mContextMenu->addAction(mUiZoomToRectAct);
}

void MainWindow::CreateToolbars()
{
    // create edit toolbar
    mUiEditToolBar = addToolBar(tr("edit"));
    mUiEditToolBar->setIconSize(QSize(24,24));
    mUiEditToolBar->addAction(mUiCopyAct);
    mUiEditToolBar->addAction(mUiPasteAct);
    mUiEditToolBar->addAction(mUiCutAct);

    mUiEditToolBar->addAction(mUiUndoAct);
    mUiEditToolBar->addAction(mUiRedoAct);

    mUiImgBrowseControlToolBar = addToolBar(tr("browse"));

    mUiImgBrowseControlToolBar->addAction(mUiPanAct);
    mUiImgBrowseControlToolBar->addAction(mUiZoomInAct);
    mUiImgBrowseControlToolBar->addAction(mUiZoomOutAct);
    mUiImgBrowseControlToolBar->addAction(mUiZoomToRectAct);
    mUiImgBrowseControlToolBar->addAction(mUiZoomFitAct);
    mUiImgBrowseControlToolBar->addAction(mUiZoomOneAct);

    // create draw toolbar
    mUiDrawToolBar = addToolBar(tr("drawing"));
    mUiDrawToolBar->setIconSize(QSize(24,24));
    mUiDrawToolBar->addAction(mUiSelectAct);

    mUiDrawToolBar->addAction(mUiRectAct);
    mUiDrawToolBar->addAction(mUiRoundRectAct);
    mUiDrawToolBar->addAction(mUiEllipseAct);
    mUiDrawToolBar->addAction(mUiLineAct);
    mUiDrawToolBar->addAction(mUiPolylineAct);
    mUiDrawToolBar->addAction(mUiPolygonAct);
    //mUiDrawToolBar->addAction(mUiBezierAct);
    mUiDrawToolBar->addAction(mUiRotateAct);
    mUiDrawToolBar->addAction(mUiSelectColorAct);

    // create align toolbar
#if 0
    mUiAlignToolBar = addToolBar(tr("align"));
    mUiAlignToolBar->setIconSize(QSize(24,24));
    mUiAlignToolBar->addAction(mUiUpAct);
    mUiAlignToolBar->addAction(mUiDownAct);
    mUiAlignToolBar->addAction(mUiRightAct);
    mUiAlignToolBar->addAction(mUiLeftAct);
    mUiAlignToolBar->addAction(mUiVCenterAct);
    mUiAlignToolBar->addAction(mUiHCenterAct);

    mUiAlignToolBar->addAction(mUiHorzAct);
    mUiAlignToolBar->addAction(mUiVertAct);
    mUiAlignToolBar->addAction(mUiHeightAct);
    mUiAlignToolBar->addAction(mUiWidthAct);
    mUiAlignToolBar->addAction(mUiAllAct);

    mUiAlignToolBar->addAction(mUiBringToFrontAct);
    mUiAlignToolBar->addAction(mUiSendToBackAct);
    mUiAlignToolBar->addAction(mUiGroupAct);
    mUiAlignToolBar->addAction(mUiUnGroupAct);
#endif
}

void MainWindow::CreateToolBox()
{
    
}

void MainWindow::CreatePropertyEditor()
{
    mUiDockProperty = new QDockWidget(this);
    addDockWidget(Qt::RightDockWidgetArea, mUiDockProperty);

    mPropertyEditor = new ObjectController(this);
    mUiDockProperty->setWidget(mPropertyEditor);
}

void MainWindow::CreateStatusBar()
{
    mUiMousePosFromViewInfo = new QLabel(tr(" view:  "));
    mUiMousePosFromViewInfo->setMinimumWidth(100);
    mUiMousePosFromViewInfo->setAlignment(Qt::AlignHCenter);
    statusBar()->addWidget(mUiMousePosFromViewInfo);

    connect(mView, &InteractiveView::posFromViewChanged,
            this, &MainWindow::OnPosFromViewChanged);

    mUiMousePosFromSceneInfo = new QLabel(tr(" scene:  "));
    mUiMousePosFromSceneInfo->setMinimumWidth(100);
    mUiMousePosFromSceneInfo->setAlignment(Qt::AlignHCenter);
    statusBar()->addWidget(mUiMousePosFromSceneInfo);

    connect(mView, &InteractiveView::posFromSceneChanged,
            this, &MainWindow::OnPosFromSceneChanged);

    mUiScaleInfo = new QLabel(tr(" scale: 100% "));
    mUiScaleInfo->setMinimumWidth(80);
    mUiScaleInfo->setAlignment(Qt::AlignHCenter);
    statusBar()->addWidget(mUiScaleInfo);

    connect(mView, &InteractiveView::scaleChanged,
            this, &MainWindow::OnScaleChanged);
}

void MainWindow::CreateImageListView()
{
    mUiDockImageList = new QDockWidget(tr("File List"), this);
    addDockWidget(Qt::RightDockWidgetArea, mUiDockImageList);

    mUiImageListView = new QListView(this);
    mImageFileListModel = new ImageFileListModel(this);
    mUiDockImageList->setWidget(mUiImageListView);

    mUiImageListView->setModel(mImageFileListModel);
    mUiImageListView->setEditTriggers(QListView::EditTrigger::NoEditTriggers);

    connect(mUiImageListView, &QListView::doubleClicked,
            this, &MainWindow::OnImageListViewDoubleClicked);
}

void MainWindow::InitGraphicsView()
{
    //--------------------------------------------------------
    // 创建Scene
    mScene = new GraphicsScene(this);
    //QRectF rc = QRectF(0 , 0 , 800, 600);
    //mScene->setSceneRect(rc);
    
    //qDebug()<<rc.bottomLeft()<<rc.size() << rc.topLeft();

    connect(mScene, SIGNAL(selectionChanged()),
            this, SLOT(OnItemSelected()));

    connect(mScene,SIGNAL(itemAdded(QGraphicsItem*)),
            this, SLOT(OnItemAdded(QGraphicsItem*)));
    connect(mScene,SIGNAL(itemMoved(QGraphicsItem*,QPointF)),
            this,SLOT(OnItemMoved(QGraphicsItem*,QPointF)));
    connect(mScene,SIGNAL(itemRotate(QGraphicsItem*,qreal)),
            this,SLOT(OnItemRotate(QGraphicsItem*,qreal)));

    connect(mScene,SIGNAL(itemResize(QGraphicsItem* , int , const QPointF&)),
            this,SLOT(OnItemResize(QGraphicsItem* , int , const QPointF&)));

    connect(mScene,SIGNAL(itemControl(QGraphicsItem* , int , const QPointF&,const QPointF&)),
            this,SLOT(OnItemControl(QGraphicsItem* , int , const QPointF&,const QPointF&)));

    //----------------------------------------------------------------
    // 创建 View
    mView = new InteractiveView(this);
    mView->setScene(mScene);
    mScene->setView(mView);
    connect(mView,SIGNAL(posFromSceneChanged(double,double)),this,SLOT(OnPosFromSceneChanged(double,double)));

#if 0
    mView->setRenderHint(QPainter::Antialiasing);
    mView->setCacheMode(QGraphicsView::CacheBackground);
    mView->setOptimizationFlags(QGraphicsView::DontSavePainterState);
    mView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    //view->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    mView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    // TODO 将原本左上角为原点设置为左下角为原点
    // move orign point to leftbottom
    //view->setTransform(view->transform().scale(1,-1));

    //mView->setBackgroundBrush(Qt::darkGray);
#endif
    
    //QGridLayout *layout = new QGridLayout;
    //layout->addWidget(mView);
    //centralWidget()->setLayout(layout);
    setCentralWidget(mView);

    // TODO 鹰眼图
    mEyeView = new InteractiveView;
    mEyeView->setScene(mScene);
    mEyeView->SetRuleBarVisiable(false);
    mEyeView->setMinimumSize(256,256);
    mEyeView->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);

    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addWidget(mEyeView);
    ui->dockWidgetContents_EyeView->setLayout(hLayout);
    ui->dockWidget_EyeView->setVisible(false);
}

void MainWindow::UpdateImageFileListModel(const QStringList &fileList)
{
    mImageFileListModel->setStringList(fileList);
}

void MainWindow::UpdateScene(const QString &imgFile)
{
    // TODO 更新场景，每次双击图片时，更换场景内容
    QFileInfo file(imgFile);
    QImageReader imgReader(imgFile, file.suffix().toLocal8Bit());
    QImage img = imgReader.read();

    mScene->clear();
    mScene->addPixmap(QPixmap::fromImage(img));
    mView->FitInView(0, 0, img.width(), img.height());
    //mView->Zoom1To1(0, 0, img.width(), img.height());
}

Layer::Layer(const QColor &color, QGraphicsItem *parent)
    : QGraphicsItemGroup(parent) {

    mRectItem = new QGraphicsRectItem(0, 0, 640, 480);
    mRectItem->setBrush(color);
    addToGroup(mRectItem);

    // 各个child item 处理自己的事件
    setHandlesChildEvents(false);

    QPen p = mRectItem->pen();
    p.setWidth(0);
    p.setColor(Qt::red);

    // 去除外边框
    mRectItem->setPen(QPen(Qt::NoPen));

    // QGraphicsItem::ItemClipsChildrenToShape 隐藏子Item超出部分
    setFlags(QGraphicsItem::ItemClipsChildrenToShape);
}

Layer::Layer(double x, double y, int w, int h, const QColor &color, QGraphicsItem *parent)
    : QGraphicsItemGroup(parent) {
    mRectItem = new QGraphicsRectItem(x, y, w, h);
    mRectItem->setBrush(color);
    addToGroup(mRectItem);

    // 各个child item 处理自己的事件
    setHandlesChildEvents(false);

    // 去除外边框
    mRectItem->setPen(QPen(Qt::NoPen));

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

QRectF Layer::GetRectFromScene()
{
    return this->sceneBoundingRect();
}

