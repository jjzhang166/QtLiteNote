#-------------------------------------------------
#
# Project created by QtCreator 2016-02-26T16:29:23
#
#-------------------------------------------------

QT       += webkitwidgets core gui   widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtLiteNote
TEMPLATE = app


SOURCES +=\
    ../src/MainLiteNote.cpp \
    ../src/Markdown.cpp \
    ../src/Markdown_inc.cpp \
    ../src/PCString.cpp \
    ../src/QFileEx.cpp \
    ../src/QLiteNoteWindow.cpp \
    ../src/QTreeWidgetEx.cpp \
    ../src/ras_ras.cpp

HEADERS  += \
    ../src/Markdown.h \
    ../src/PCString.h \
    ../src/QFileEx.h \
    ../src/QLiteNoteWindow.h \
    ../src/QtHead.h \
    ../src/QTreeWidgetEx.h

LIBS += -lLnWin_mtd_vs2013 -lLnCore_mtd_vs2013
