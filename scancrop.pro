# -------------------------------------------------
# Project created by QtCreator 2009-05-13T21:44:43
# -------------------------------------------------
QT += xml opengl
TARGET = scancrop
TEMPLATE = app
CONFIG += debug
SOURCES += main.cpp \
	mainwindow.cpp \
	imagewidget.cpp \
	pdfscene.cpp \
	tool.cpp \
	boxtool.cpp \
	selecttool.cpp \
	rectangle.cpp
HEADERS += mainwindow.h \
	imagewidget.h \
	common.h \
	pdfscene.h \
	tool.h \
	boxtool.h \
	selecttool.h \
	rectangle.h
FORMS += mainwindow.ui

# LIBS += -lpoppler-qt4
# LIBS += -L/usr/local/lib \
# -lpoppler-qt4 \
# -lpoppler
# ~ LIBS += libpoppler.so.5 \
# ~ libpoppler-qt4.so
# INCLUDEPATH += /usr/local/include/poppler/qt4
RESOURCES += icons.qrc
