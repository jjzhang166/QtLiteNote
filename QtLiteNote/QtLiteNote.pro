#-------------------------------------------------
#
# Project created by QtCreator 2016-02-26T16:29:23
#
#-------------------------------------------------

QT       += webkitwidgets webkit core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtLiteNote
TEMPLATE = app


SOURCES +=\
    ../src/MainLiteNote.cpp \
    ../src/PCString.cpp \
    ../src/QFileEx.cpp \
    ../src/QLiteNoteWindow.cpp \
    ../src/QTreeWidgetEx.cpp \
    ../src/ras_ras.cpp \
    ../src/MkNode.cpp \
    ../src/MarkdownThread.cpp

HEADERS  += \
    ../src/PCString.h \
    ../src/QFileEx.h \
    ../src/QLiteNoteWindow.h \
    ../src/QtHead.h \
    ../src/QTreeWidgetEx.h \
    ../src/MkNode.h \
    ../src/MarkdownThread.h

win32 {
        RC_FILE = LiteNote.rc
        DISTFILES += \
            LiteNote.rc
        SOURCES += \
            ../src/utility.cpp
        HEADERS += \
            ../src/utility.h
}
macx {
        ICON=app.icns
#        QMAKE_RPATHDIR += @executable_path/../../Frameworks
}
CONFIG += c++11


#QMAKE_CFLAGS_RELEASE -= -MD
#QMAKE_CXXFLAGS_RELEASE -= -MD
#QMAKE_CFLAGS_RELEASE += /MT
#QMAKE_CXXFLAGS_RELEASE += /MT

#QMAKE_CFLAGS_DEBUG += /MTd
#QMAKE_CXXFLAGS_DEBUG += /MTd
#QMAKE_CFLAGS_DEBUG -= -MDd
#QMAKE_CXXFLAGS_DEBUG -= -MDd
#INCLUDEPATH += $$PWD/../../LiteNice/include
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../LiteNice/lib/ -lLnCore_vs2013_MDd -lLnWin_vs2013_MDd

#INCLUDEPATH += $$PWD/../../LiteNice/include
