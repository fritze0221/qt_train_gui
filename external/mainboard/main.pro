QT       += core
QT       -= gui
QT += core network

CONFIG   += console
CONFIG   -= app_bundle

SOURCES += main.cpp \
        mainboard_handle.cpp\
        ../src/apihandler.cpp\
        ../api/src_winterface/winterface-api.cpp\
        ../api/src_winterface/winterface.cpp

HEADERS +=  mainboard_handle.h\
        ../src/apihandler.h\
        ../api/src_winterface/winterface-api.h\
        ../api/src_winterface/winterface.h
        
TARGET = mainboard

DESTDIR = /home/urs/uni/cpp-gui/qt_gui/build/Api-Build/

# Optional: Zusätzliche Compiler-Optionen
# QMAKE_CXXFLAGS += -Wall -Wextra
