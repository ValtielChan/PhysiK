TARGET = PhysiK
TEMPLATE = lib
CONFIG += staticlib
CONFIG += c++11

QMAKE_CXXFLAGS += -std=c++11

DESTDIR = $$lib_dir

QT -= core gui

INCLUDEPATH += ../

SOURCES += triangle.cpp \
    physicobject.cpp \
    vec3.cpp \
    body.cpp \
    particlegroup.cpp \
    constraint.cpp \
    particlesystem.cpp \
    trianglehashtable.cpp \
    solver.cpp \
    intersection.cpp \
    particle.cpp

HEADERS += \
    triangle.h \
    physicobject.h \
    vec3.h \
    body.h \
    particlegroup.h \
    constraint.h \
    particlesystem.h \
    trianglehashtable.h \
    solver.h \
    intersection.h \
    particle.h

header_files.files = $$HEADERS
header_files.path = $$lib_dir/include
INSTALLS += header_files

