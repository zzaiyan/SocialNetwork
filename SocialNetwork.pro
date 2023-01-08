QT       += core gui svg
qtHaveModule(opengl): QT += opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    canvaswidget.cpp \
    launcher.cpp \
    main.cpp \
    mygraphicsview.cpp \
    rel.cpp \
    role.cpp

HEADERS += \
    canvaswidget.h \
    launcher.h \
    mygraphicsview.h \
    net.h \
    node.h \
    rel.h \
    role.h

FORMS += \
    canvaswidget.ui \
    launcher.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

DISTFILES += \
    hlm.csv
