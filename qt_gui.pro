QT += core gui
QT += core network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/apihandler.cpp \
    src/couplebutton.cpp \
    src/mainboardcontrol.cpp \
    src/main.cpp \
    src/mainwindow.cpp\
    src/stopbutton.cpp \
    src/traincontrol.cpp\
    src/connectioncontrol.cpp

HEADERS += \
    src/apihandler.h \
    src/couplebutton.h \
    src/mainboardcontrol.h \
    src/mainwindow.h\
    src/stopbutton.h \
    src/traincontrol.h\
    src/connectioncontrol.h\
    src/config.h\
    external/api/src_winterface/winterface-config.h

INCLUDEPATH += external/api/src_winterface/


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

FORMS += \
    src/mainboardcontrol.ui

RESOURCES += \
    src/rec.qrc
