QT       += core
QT       -= gui
QT += core network

CONFIG   += console
CONFIG   -= app_bundle

SOURCES += main.cpp \
        train_2_handle.cpp\
        ../src/apihandler.cpp\
        ../api/src_winterface/winterface-api.cpp\
        ../api/src_winterface/winterface.cpp

HEADERS +=  train_2_handle.h\
        ../src/apihandler.h\
        ../api/src_winterface/winterface-api.h\
        ../api/src_winterface/winterface.h

TARGET = train2

DESTDIR = /home/urs/uni/cpp-gui/qt_gui/build/Api-Build/

# Optional: Zusätzliche Compiler-Optionen
# QMAKE_CXXFLAGS += -Wall -Wextra
