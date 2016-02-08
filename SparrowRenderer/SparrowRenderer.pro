TARGET = SparrowRenderer
TEMPLATE = lib
CONFIG += staticlib
CONFIG += c++11

QMAKE_CXXFLAGS += -std=c++11

DESTDIR = $$lib_dir

QT -= core gui

CFLAGS += -Wcomment

DEFINES += GLEW_STATIC

INCLUDEPATH += ../

win32 : SOURCES += glew.c \

SOURCES += crappymodule.cpp \
	forwardmodule.cpp \
	framebuffer.cpp \
	light.cpp \
	mesh.cpp \
	meshbuilder.cpp \
	parametricmesh.cpp \
	phongmaterial.cpp \
	scene.cpp \
	shader.cpp \
	shadersource.cpp \
	sparrowrenderer.cpp \
	texture.cpp

HEADERS += camera.h \
	crappymodule.h \
	entity.h \
	forwardmodule.h \
	framebuffer.h \
	glassert.h \
	glew.h \
	image.h \
	light.h \
	material.h \
	mesh.h \
	meshbuilder.h \
	module.h \
	parametricmesh.h \
	phongmaterial.h \
	scene.h \
	shader.h \
	shadersource.h \
	sparrowrenderer.h \
	texture.h \
	wglew.h
	
	
