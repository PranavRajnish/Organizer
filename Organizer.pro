QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    circleedit.cpp \
    customlineedit.cpp \
    customtextedit.cpp \
    databasehelper.cpp \
    displayframe.cpp \
    draggable.cpp \
    draggableobjects.cpp \
    logindialog.cpp \
    main.cpp \
    mainwindow.cpp \
    sidebar.cpp \
    todoitem.cpp

HEADERS += \
    circleedit.h \
    customlineedit.h \
    customtextedit.h \
    databasehelper.h \
    displayframe.h \
    draggable.h \
    draggableobjects.h \
    logindialog.h \
    mainwindow.h \
    sidebar.h \
    todoitem.h

FORMS += \
    logindialog.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icons.qrc \
    styleresource.qrc
