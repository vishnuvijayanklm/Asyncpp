#-------------------------------------------------
#
# Project created by QtCreator 2018-06-25T12:54:59
#
#-------------------------------------------------

QT       += core gui

TARGET = Framework
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11

SOURCES += \
    util/src/File.cpp \
    util/src/Option.cpp \
    util/src/Runnable.cpp \
    util/src/TimerManager.cpp \
    core/src/Application.cpp \
    core/src/Exception.cpp \
    core/src/Notifier.cpp \
    core/src/NotifyManager.cpp \
    main.cpp \
    util/src/Logger.cpp \
    core/src/SharedLibraryLoader.cpp \
    core/src/SharedLibraryManager.cpp

HEADERS  += \
    util/include/File.h \
    util/include/Option.h \
    util/include/Runnable.h \
    util/include/TimerManager.h \
    core/include/Application.h \
    core/include/EventInfo.h \
    core/include/Exception.h \
    core/include/Notifier.h \
    core/include/NotifyManager.h \
    core/include/Subsystem.h \
    containers/include/Stl.h \
    util/include/Logger.h \
    util/src/defines.h \
    util/include/defines.h \
    util/include/SharedLibraryLoader.h \
    core/include/SharedLibraryLoader.h \
    core/include/SharedLibraryManager.h

INCLUDEPATH += \
    util/include \
    core/include \
    containers/include/
