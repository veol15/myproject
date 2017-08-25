#-------------------------------------------------
#
# Project created by QtCreator 2017-08-24T16:52:55
#
#-------------------------------------------------

QT       += core gui opengl sql svg network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mydemo
TEMPLATE = app

DESTDIR  = ../bin
CONFIG(debug, debug|release){
    TARGET = $$join(TARGET,,,d)
}
# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    MapWidget.cpp \
    UAS_types.cpp \
    wayopwgt.cpp

HEADERS += \
        mainwindow.h \
    MapWidget.h \
    UAS_types.h \
    wayopwgt.h

FORMS += \
        mainwindow.ui \
    wayopwgt.ui

OPMAPCONTROL_DIR = ../..
INCLUDEPATH +=  $$OPMAPCONTROL_DIR \
                $$OPMAPCONTROL_DIR/core \
                $$OPMAPCONTROL_DIR/internals \
                $$OPMAPCONTROL_DIR/mapwidget
CONFIG(debug, debug|release) {
LIBS += $$PWD/../../libs/libopmapwidgetd.a
} else {
LIBS += $$PWD/../../libs/libopmapwidget.a
}

RESOURCES += \
    mydemo.qrc

DISTFILES +=
