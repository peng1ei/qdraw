#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "GraphicsViewFramework/GraphicsView.h"
#include "GraphicsViewFramework/GraphicsScene.h"
#include "GraphicsViewFramework/ObjectController.h"
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

QT_BEGIN_NAMESPACE
class QAction;
class QActionGroup;
class QToolBox;
class QSpinBox;
class QComboBox;
class QFontComboBox;
class QButtonGroup;
class QLineEdit;
class QGraphicsTextItem;
class QFont;
class QToolButton;
class QAbstractButton;
class QGraphicsView;
class QListWidget;
class QStatusBar;
class QMdiArea;
class QMdiSubWindow;
class QSignalMapper;
class QUndoStack;
class QUndoView;
class QLabel;
class QResizeEvent;
class QListView;
class QStringListModel;
class QEvent;
QT_END_NAMESPACE

class ProgressBarDialog;
class ObjectController;
class GraphicsScene;
class ImageFileListModel;
class ColorCombox;
class ImageFileListProviderThd;

class Layer : public QGraphicsItemGroup {
public:
    explicit Layer(const QColor &color = Qt::white, QGraphicsItem *parent = nullptr);
    Layer(double x, double y, int w, int h, const QColor &color = Qt::white, QGraphicsItem *parent = nullptr);
    ~Layer();

    void AddItem(QGraphicsItem *item);

    QRectF GetRectFromScene();

private:
    QGraphicsRectItem *mRectItem;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    
protected:
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
    bool eventFilter(QObject* obj, QEvent *evt) override;

public slots:
    void OnAddLayer();
    void OnLayerVisiableChanged(int status);
    void OnOpacityChanged(int value);
    
    //----------------------------
    // Action
    void OnAddShape();
    void OnSelectColor();

    void OnNewFile();
    void OnOpen();
    void OnOpenDir();
    void OnSave();
    
    void OnAbout();
    void OnFuncTestTriggered();
    
    void OnBringToFrontTriggered();
    void OnSendToBackTriggered();
    void OnAglinTriggered();
    void OnGroupTriggered();
    void OnUnGroupTriggered();
    
    void OnCopy();
    void OnPaste();
    void OnCut();
    void OnZoomIn();
    void OnZoomOut();
    void OnZoomFitView();
    void OnZoomOne();
    void OnZoomToRect();
    void OnPan();
    void OnDeleteItem();
    
    // from item
    void OnItemSelected();
    void OnItemMoved( QGraphicsItem * item , const QPointF & oldPosition );
    void OnItemRotate(QGraphicsItem * item , const qreal oldAngle );
    void OnItemAdded(QGraphicsItem * item );
    void OnItemResize(QGraphicsItem * item , int handle , const QPointF& scale );
    void OnItemControl(QGraphicsItem * item , int handle , const QPointF & newPos , const QPointF& lastPos_ );
    
    // from graphicsview
    void OnScaleChanged(double scale);
    void OnPosFromSceneChanged(double x, double y);
    void OnPosFromViewChanged(int x, int y);

    // import image
    void OnOpenDirFinished();
    void OnOpenDirResult(bool, QString);

    void OnImageListViewDoubleClicked(const QModelIndex &index);

    void OnPenColorChanged(QColor color);
    void OnBrushColorChanged(QColor color);

private:
    void SetLayerVisiable(Layer *layer, bool visiable);
    
    void InitUI();
    void CreateActions();
    void CreateMenus();
    void CreateToolbars();
    void CreateToolBox();
    void CreatePropertyEditor();
    void CreateStatusBar();
    void CreateImageListView();
    
    void InitGraphicsView();

    void UpdateImageFileListModel(const QStringList &fileList);
    void UpdateScene(const QString &imgFile);

private:
    Ui::MainWindow *ui;

    Layer *mLayer0;
    Layer *mLayer1;

    gvf::GraphicsView *mView;
    gvf::GraphicsScene *mScene;

    gvf::GraphicsView *mEyeView;
    
    // Menu and ToolBar
    QMenu *mUiMenu;
    
    // edit toolbar;
    QToolBar * mUiEditToolBar;

    // align toolbar
    QToolBar * mUiAlignToolBar;
    QAction  * mUiRightAct;
    QAction  * mUiLeftAct;
    QAction  * mUiVCenterAct;
    QAction  * mUiHCenterAct;
    QAction  * mUiUpAct;
    QAction  * mUiDownAct;
    QAction  * mUiHorzAct;
    QAction  * mUiVertAct;
    QAction  * mUiHeightAct;
    QAction  * mUiWidthAct;
    QAction  * mUiAllAct;
    QAction  * mUiBringToFrontAct;
    QAction  * mUiSendToBackAct;

    QAction  * mUiFuncAct;
    // file
    QAction *mUiNewAct;
    QAction *mUiOpenAct;
    QAction *mUiOpenDirAct;
    QAction *mUiSaveAct;
    QAction *mUiExitAct;

    QAction  * mUiGroupAct;
    QAction  * mUiUnGroupAct;

    // edit action
    QAction  * mUiDeleteAct;
    QAction  * mUiUndoAct;
    QAction  * mUiRedoAct;
    QAction  * mUiCopyAct;
    QAction  * mUiPasteAct;
    QAction  * mUiCutAct;

    // image browse control
    QToolBar * mUiImgBrowseControlToolBar;
    QAction  * mUiPanAct;
    QAction  * mUiZoomInAct;
    QAction  * mUiZoomOutAct;
    QAction  * mUiZoomFitAct;
    QAction  * mUiZoomOneAct;
    QAction  * mUiZoomToRectAct;

    // drawing toolbar
    QToolBar * mUiDrawToolBar;
    QActionGroup * mUiDrawActionGroup;
    QAction  * mUiSelectAct;
    QAction  * mUiLineAct;
    QAction  * mUiRectAct;
    QAction  * mUiRoundRectAct;
    QAction  * mUiEllipseAct;
    QAction  * mUiPolygonAct;
    QAction  * mUiPolylineAct;
    QAction  * mUiBezierAct;
    QAction  * mUiRotateAct;
    QAction  * mUiSelectColorAct;

    ColorCombox * mUiPenColorCombox;
    ColorCombox * mUiBrushColorCombox;

    QAction *mUiCloseAct;
    QAction *mUiCloseAllAct;
    QAction *mUiTileAct;
    QAction *mUiCascadeAct;
    QAction *mUiNextAct;
    QAction *mUiPreviousAct;
    QAction *mUiSeparatorAct;
    QAction *mUiAboutAct;
    QAction *mUiAboutQtAct;
    
    QListWidget    *mUiListView;

    QUndoStack *mUiUndoStack;
    QUndoView *mUiUndoView;
    // statusbar label
    QLabel *mUiMousePosFromSceneInfo;
    QLabel  *mUiMousePosFromViewInfo;
    QLabel *mUiScaleInfo;
    
    //property editor
    QDockWidget *mUiDockProperty;
    gvf::ObjectController *mPropertyEditor;
    QObject *mTheControlledObject;

    // QDockWidget imagelist
    QDockWidget *mUiDockImageList;
    QListView *mUiImageListView;
    ImageFileListModel *mImageFileListModel;

    ProgressBarDialog *mProgressBarDlg;
    ImageFileListProviderThd *mImageFileListProviderThd;

    QMenu *mContextMenu = nullptr;
    ProgressBarDialog *mTestDlg;
};
#endif // MAINWINDOW_H
