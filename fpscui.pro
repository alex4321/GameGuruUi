#-------------------------------------------------
#
# Project created by QtCreator 2015-07-28T18:25:38
#
#-------------------------------------------------

QT += core widgets  webkitwidgets
CONFIG += c++11

TARGET = fpscui
TEMPLATE = lib

DEFINES += FPSCUI_LIBRARY

SOURCES += fpscui.cpp \
    uilayer.cpp \
    uiblock.cpp \
    table.cpp \
    uithread.cpp \
    uitimer.cpp \
    tablebinding.cpp \
    modallayerwindow.cpp \
    helper.cpp

HEADERS += fpscui.h\
    include/lauxlib.h \
    include/lua.h \
    include/lua.hpp \
    include/luaconf.h \
    include/lualib.h \
    uilayer.h \
    uiblock.h \
    table.h \
    uithread.h \
    uitimer.h \
    tablebinding.h \
    modallayerwindow.h \
    helper.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/ -llua52
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/ -llua52
else:unix: LIBS += -llua52

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include
