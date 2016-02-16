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
QMAKE_CXXFLAGS += -Wno-comment #to much warning... unreadable console

#QMAKE_LFLAGS += -static-libgcc -static-libstdc++ -static -lpthread

DESTDIR = $$bin_dir

DEFINES += GLEW_STATIC

LIBPATH += $$lib_dir

LIBS += -lPhysiK -lSparrowRenderer -fopenmp
win32 : LIBS += -lopengl32

INCLUDEPATH += ../

SOURCES += main.cpp\
    mainwindow.cpp \
    drawwidget.cpp \
    mycamera.cpp \
    qtutils.cpp \
    wavefrontmesh.cpp \
    scenemanager.cpp \
    particledialog.cpp \
    particlemesh.cpp \
    statusbar.cpp \
    pickmodule.cpp

HEADERS  += mainwindow.h \
    drawwidget.h \
    mycamera.h \
    qtutils.h \
    wavefrontmesh.h \
    scenemanager.h \
    particledialog.h \
    particlemesh.h \
    statusbar.h \
    pickmodule.h

FORMS    += mainwindow.ui

RESOURCES += resources.qrc
