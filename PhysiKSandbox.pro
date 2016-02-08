QT       += core gui widgets opengl

TEMPLATE = subdirs
CONFIG += ordered c++11
SUBDIRS = SparrowRenderer \
	PhysiK \
	app

app.depends = SparrowRenderer PhysiK

