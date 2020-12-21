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
    
    //----------------------------
    // Action
    void OnAddShape();

    void OnNewFile();
    void OnOpen();
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
    void OnDeleteItem();
    
private:
    void SetLayerVisiable(Layer *layer, bool visiable);
    
    void InitUI();
    void CreateActions();
    void CreateMenus();
    void CreateToolbars();
    void CreateToolBox();
    void CreatePropertyEditor();

private:
    Ui::MainWindow *ui;

    Layer *mLayer0;
    Layer *mLayer1;

    InteractiveView *mView;
    QGraphicsScene *mScene;
    
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
    QAction  * mUiZoomInAct;
    QAction  * mUiZoomOutAct;

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
    QLabel *mMousePosFromSceneInfo;
    QLabel  *mMousePosFromViewInfo;
};
#endif // MAINWINDOW_H
