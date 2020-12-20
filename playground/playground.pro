QT       += core gui opengl widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

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
QMAKE_CXXFLAGS_WARN_ON += -wd4819

SOURCES += \
    GraphicsView.cpp \
    RuleBar.cpp \
    main.cpp \
    MainWindow.cpp

HEADERS += \
    GraphicsView.h \
    MainWindow.h \
    RuleBar.h

FORMS += \
    MainWindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
