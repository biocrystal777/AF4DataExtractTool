#-------------------------------------------------
#
# Project created by QtCreator 2017-10-11T22:07:35
#
#-------------------------------------------------

QT       += core gui svg

greaterThan(QT_MAJOR_VERSION, 5): QT += widgets

QMAKE_CXXFLAGS += -fopenmp
QMAKE_LFLAGS += -fopenmp
QMAKE_CXXFLAGS += -O0
QMAKE_LFLAGS += -O0

DESTDIR = ../Output
MOC_DIR = ../moc
OBJECTS_DIR = ../obj

TARGET = ExtractTool
TEMPLATE = app


# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    fffcsvparser.cpp \
    ffflog_omp.cpp \
    ffffileinoutwidget.cpp \
    fffcsvwriter.cpp

HEADERS  += mainwindow.h \
    fffcsvparser.h \
    fffdatatypes.h \
    ffflog_omp.h \
    ffffileinoutwidget.h \
    fffcsvwriter.h

FORMS    +=
