#-------------------------------------------------
#
# Project created by QtCreator 2015-06-13T23:45:28
#
#-------------------------------------------------

QT       += core gui xml svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

include(../qtpropertybrowser/src/qtpropertybrowser.pri)
TARGET = qdraw
TEMPLATE = app

CONFIG(debug, debug|release){
    DESTDIR = $$PWD/../bin/debug

    PF_BUILD_DIR = $$PWD/../build/debug
    MOC_DIR = $${PF_BUILD_DIR}/moc
    RCC_DIR = $${PF_BUILD_DIR}/rcc
    UI_DIR = $${PF_BUILD_DIR}/ui
    OBJECTS_DIR = $${PF_BUILD_DIR}/obj
}else{
    DESTDIR = $$PWD/../bin/release

    PF_BUILD_DIR = $$PWD/../build/release
    MOC_DIR = $${PF_BUILD_DIR}/moc
    RCC_DIR = $${PF_BUILD_DIR}/rcc
    UI_DIR = $${PF_BUILD_DIR}/ui
    OBJECTS_DIR = $${PF_BUILD_DIR}/obj
}

SOURCES += main.cpp\
        mainwindow.cpp \
    drawobj.cpp \
    drawscene.cpp \
    drawtool.cpp \
    sizehandle.cpp \
    objectcontroller.cpp \
    customproperty.cpp \
    rulebar.cpp \
    drawview.cpp \
    commands.cpp

HEADERS  += mainwindow.h \
    drawobj.h \
    drawscene.h \
    drawtool.h \
    sizehandle.h \
    objectcontroller.h \
    customproperty.h \
    rulebar.h \
    drawview.h \
    commands.h

RESOURCES += \
    app.qrc
