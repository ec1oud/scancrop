QT += widgets xml
TARGET = scancrop
TEMPLATE = app
CONFIG += debug

SOURCES += main.cpp \
    mainwindow.cpp \
    imagescanner.cpp \
    imagewidget.cpp \
    tool.cpp \
    boxtool.cpp \
    selecttool.cpp \
    settings.cpp \
    preferencesdialog.cpp \
    rectangle.cpp \
    mainimagescene.cpp \

HEADERS += mainwindow.h \
    imagescanner.h \
    imagewidget.h \
    common.h \
    tool.h \
    boxtool.h \
    selecttool.h \
    settings.h \
    preferencesdialog.h \
    rectangle.h \
    mainimagescene.h \

FORMS += mainwindow.ui preferencesdialog.ui

!isEmpty(OPENCV_DIR) {
    INCLUDEPATH += $$OPENCV_DIR/include
    LIBS += -L $$OPENCV_DIR/lib
}

INCLUDEPATH += /usr/include/opencv4
LIBS += -lopencv_core -lopencv_imgproc -lopencv_objdetect -lsane

RESOURCES += icons.qrc
