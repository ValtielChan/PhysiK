#-------------------------------------------------
#
# Project created by QtCreator 2016-02-08T15:01:19
#
#-------------------------------------------------

QT       += core gui widgets opengl

TARGET = PhysiKSandbox
TEMPLATE = app
CONFIG += c++11

QMAKE_CXXFLAGS += -std=c++11

DESTDIR = $$bin_dir

DEFINES += GLEW_STATIC

LIBPATH += $$lib_dir

LIBS += -lPhysiK -lSparrowRenderer
unix {
    LIBS += -lGL
}
win32 {
    LIBPATH += $$lib_dir/win32
    LIBS += -lopengl32
}

INCLUDEPATH += ../

SOURCES += main.cpp\
    mainwindow.cpp \
    drawwidget.cpp \
    mycamera.cpp \
    qtutils.cpp \
    wavefrontmesh.cpp \
    scenemanager.cpp

HEADERS  += mainwindow.h \
    drawwidget.h \
    mycamera.h \
    qtutils.h \
    wavefrontmesh.h \
    scenemanager.h

FORMS    += mainwindow.ui
