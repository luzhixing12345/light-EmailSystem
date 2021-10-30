QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    CBase64.cpp \
    main.cpp \
    mainwindow.cpp \
    receive_window.cpp \
    send_window.cpp

HEADERS += \
    CBase64.h \
    mainwindow.h \
    receive_window.h \
    send_window.h

FORMS += \
    mainwindow.ui \
    receive_window.ui \
    send_window.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

LIBS += -lpthread libwsock32 libws2_32


