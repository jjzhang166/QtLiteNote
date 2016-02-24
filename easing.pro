QT += widgets

HEADERS = \
    src/Markdown.h \
    src/PCString.h \
    src/QFileEx.h \
    src/QLiteNoteWindow.h \
    src/QtHead.h \
    src/QTreeWidgetEx.h \
    src/resource.h
SOURCES = \
    src/MainLiteNote.cpp \
    src/Markdown_inc.cpp \
    src/Markdown.cpp \
    src/PCString.cpp \
    src/QFileEx.cpp \
    src/QLiteNoteWindow.cpp \
    src/QTreeWidgetEx.cpp \
    src/ras_ras.cpp

FORMS   = form.ui \
    form.ui

RESOURCES = easing.qrc

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/animation/easing
INSTALLS += target
