QT += widgets xml
TARGET = scancrop
TEMPLATE = app
CONFIG += debug
SOURCES += main.cpp \
	mainwindow.cpp \
	imagewidget.cpp \
	tool.cpp \
	boxtool.cpp \
	selecttool.cpp \
	rectangle.cpp \
    mainimagescene.cpp
HEADERS += mainwindow.h \
	imagewidget.h \
	common.h \
	tool.h \
	boxtool.h \
	selecttool.h \
	rectangle.h \
    mainimagescene.h
FORMS += mainwindow.ui

!isEmpty(OPENCV_DIR) {
    INCLUDEPATH += $$OPENCV_DIR/include
    LIBS += -L $$OPENCV_DIR/lib
}

!contains(QT_CONFIG, no-pkg-config) {
    CONFIG += link_pkgconfig
    PKGCONFIG += opencv4
} else {
    LIBS += -lopencv_core -lopencv_imgproc -lopencv_objdetect
}

RESOURCES += icons.qrc
