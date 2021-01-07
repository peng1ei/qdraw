QT       += core gui opengl widgets xml svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

include(../qtpropertybrowser/src/qtpropertybrowser.pri)

CONFIG += c++11
TARGET = playground

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG(debug, debug|release){
    DESTDIR = $$PWD/../bin/debug

    PF_BUILD_DIR = $$PWD/../build/$${TARGET}/debug
    MOC_DIR = $${PF_BUILD_DIR}/moc
    RCC_DIR = $${PF_BUILD_DIR}/rcc
    UI_DIR = $${PF_BUILD_DIR}/ui
    OBJECTS_DIR = $${PF_BUILD_DIR}/obj
}else{
    DESTDIR = $$PWD/../bin/release

    PF_BUILD_DIR = $$PWD/../build/$${TARGET}/release
    MOC_DIR = $${PF_BUILD_DIR}/moc
    RCC_DIR = $${PF_BUILD_DIR}/rcc
    UI_DIR = $${PF_BUILD_DIR}/ui
    OBJECTS_DIR = $${PF_BUILD_DIR}/obj
}

# disable C4819 warning
win32: QMAKE_CXXFLAGS_WARN_ON += -wd4819

INCLUDEPATH += Utils/
INCLUDEPATH += Utils/ColorWidget/
INCLUDEPATH += GraphicsViewFramework/

SOURCES += \
    DrawingBoardTemplateDlg.cpp \
    GraphicsViewFramework/Commands.cpp \
    GraphicsViewFramework/CustomProperty.cpp \
    GraphicsViewFramework/DrawObj/GraphicsBezier.cpp \
    GraphicsViewFramework/DrawObj/GraphicsEllipseItem.cpp \
    GraphicsViewFramework/DrawObj/GraphicsItem.cpp \
    GraphicsViewFramework/DrawObj/GraphicsItemGroup.cpp \
    GraphicsViewFramework/DrawObj/GraphicsLineItem.cpp \
    GraphicsViewFramework/DrawObj/GraphicsPathItem.cpp \
    GraphicsViewFramework/DrawObj/GraphicsPolygonItem.cpp \
    GraphicsViewFramework/DrawObj/GraphicsRectItem.cpp \
    GraphicsViewFramework/DrawObj/GraphicsScribbleItem.cpp \
    GraphicsViewFramework/DrawObj/ShapeMimeData.cpp \
    GraphicsViewFramework/DrawObj/SizeHandle.cpp \
    GraphicsViewFramework/DrawTool/DrawTool.cpp \
    GraphicsViewFramework/DrawTool/EraserTool.cpp \
    GraphicsViewFramework/DrawTool/PanTool.cpp \
    GraphicsViewFramework/DrawTool/PolygonTool.cpp \
    GraphicsViewFramework/DrawTool/RectTool.cpp \
    GraphicsViewFramework/DrawTool/RotationTool.cpp \
    GraphicsViewFramework/DrawTool/RubberBandZoomTool.cpp \
    GraphicsViewFramework/DrawTool/ScribbleTool.cpp \
    GraphicsViewFramework/DrawTool/SelectTool.cpp \
    GraphicsViewFramework/GraphicsScene.cpp \
    GraphicsViewFramework/GraphicsView.cpp \
    GraphicsViewFramework/ObjectController.cpp \
    GraphicsViewFramework/RuleBar.cpp \
    TreeView/Layer.cpp \
    TreeView/Node.cpp \
    TreeView/TreeView.cpp \
    Utils/ColorWidget/ColorCombox.cpp \
    Utils/ColorWidget/colormodel.cpp \
    Utils/ColorWidget/qwwcolorbutton.cpp \
    Utils/ColorWidget/qwwcolorcombobox.cpp \
    Utils/ColorWidget/wwglobal_p.cpp \
    Utils/ImageFileListModel.cpp \
    Utils/ImageFileListProviderThd.cpp \
    Utils/ProgressBarDialog.cpp \
    main.cpp \
    MainWindow.cpp

HEADERS += \
    DrawingBoardTemplateDlg.h \
    GraphicsViewFramework/Commands.h \
    GraphicsViewFramework/CustomProperty.h \
    GraphicsViewFramework/DrawObj/AbstractShapeType.hpp \
    GraphicsViewFramework/DrawObj/DrawObj \
    GraphicsViewFramework/DrawObj/GraphicsBezier.h \
    GraphicsViewFramework/DrawObj/GraphicsEllipseItem.h \
    GraphicsViewFramework/DrawObj/GraphicsItem.h \
    GraphicsViewFramework/DrawObj/GraphicsItemGroup.h \
    GraphicsViewFramework/DrawObj/GraphicsLineItem.h \
    GraphicsViewFramework/DrawObj/GraphicsPathItem.h \
    GraphicsViewFramework/DrawObj/GraphicsPolygonItem.h \
    GraphicsViewFramework/DrawObj/GraphicsRectItem.h \
    GraphicsViewFramework/DrawObj/GraphicsScribbleItem.h \
    GraphicsViewFramework/DrawObj/ShapeMimeData.h \
    GraphicsViewFramework/DrawObj/SizeHandle.h \
    GraphicsViewFramework/DrawTool/DrawTool.h \
    GraphicsViewFramework/DrawTool/EraserTool.h \
    GraphicsViewFramework/DrawTool/PanTool.h \
    GraphicsViewFramework/DrawTool/PolygonTool.h \
    GraphicsViewFramework/DrawTool/RectTool.h \
    GraphicsViewFramework/DrawTool/RotationTool.h \
    GraphicsViewFramework/DrawTool/RubberBandZoomTool.h \
    GraphicsViewFramework/DrawTool/ScribbleTool.h \
    GraphicsViewFramework/DrawTool/SelectTool.h \
    GraphicsViewFramework/GraphicsScene.h \
    GraphicsViewFramework/GraphicsView.h \
    GraphicsViewFramework/ObjectController.h \
    GraphicsViewFramework/RuleBar.h \
    MainWindow.h \
    TreeView/Layer.h \
    TreeView/Node.h \
    TreeView/TreeView.h \
    Utils/ColorWidget/ColorCombox.h \
    Utils/ColorWidget/colormodel.h \
    Utils/ColorWidget/qwwcolorbutton.h \
    Utils/ColorWidget/qwwcolorcombobox.h \
    Utils/ColorWidget/wwglobal.h \
    Utils/ColorWidget/wwglobal_p.h \
    Utils/ImageFileListModel.h \
    Utils/ImageFileListProviderThd.h \
    Utils/ProgressBarDialog.h

FORMS += \
    DrawingBoardTemplateDlg.ui \
    MainWindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    playground.qrc

RC_ICONS = via.ico
