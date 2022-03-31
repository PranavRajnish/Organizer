QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Source//circleedit.cpp \
    Source//customlineedit.cpp \
    Source//customtextedit.cpp \
    Source//databasehelper.cpp \
    Source//displayframe.cpp \
    Source//draggable.cpp \
    Source//draggableobjects.cpp \
    Source//logindialog.cpp \
    Source//main.cpp \
    Source//mainwindow.cpp \
    Source//sidebar.cpp \
    Source//todoitem.cpp

HEADERS += \
    Headers//circleedit.h \
    Headers//customlineedit.h \
    Headers//customtextedit.h \
    Headers//databasehelper.h \
    Headers//displayframe.h \
    Headers//draggable.h \
    Headers//draggableobjects.h \
    Headers//logindialog.h \
    Headers//mainwindow.h \
    Headers//sidebar.h \
    Headers//todoitem.h

FORMS += \
    Forms//logindialog.ui \
    Forms//mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icons.qrc \
    styleresource.qrc
