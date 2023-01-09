QT       += core gui svg
qtHaveModule(opengl): QT += opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    canvaswidget.cpp \
    littlefamily.cpp \
    main.cpp \
    mygraphicsview.cpp \
    querywidget.cpp \
    rel.cpp \
    role.cpp \
    significancer.cpp

HEADERS += \
    canvaswidget.h \
    littlefamily.h \
    mygraphicsview.h \
    net.h \
    node.h \
    querywidget.h \
    rel.h \
    role.h \
    significancer.h

FORMS += \
    canvaswidget.ui \
    littlefamily.ui \
    querywidget.ui \
    significancer.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES +=

DISTFILES += \
    data.csv \
    data/data0.csv \
    data/id.csv \
    data/id0.csv \
    data/带0的文件供测试用 \
    icon/004.ico \
    icon/relation1.svg \
    icon/relation2.svg \
    icon/role.svg \
    icon/role.svg.bak \
    icon/role1.svg \
    icon/role2.svg \
    img/hxr.png \
    img/jj.png \
    img/jl.png \
    img/jm.png \
    img/jr.png \
    img/js.png \
    img/jtc.png \
    img/jxc.png \
    img/jyc.png \
    img/jyinc.png \
    img/jz.png \
    img/jzhen.png \
    img/ldy.png \
    img/lrh.png \
    img/pe.png \
    img/qkq.png \
    img/wfr.jpg \
    img/wxf.png \
    img/xbc.png \
    img/xfr.png \
    img/yej.png \
    pyNetwork/build/network/Analysis-00.toc \
    pyNetwork/build/network/EXE-00.toc \
    pyNetwork/build/network/PKG-00.toc \
    pyNetwork/build/network/PYZ-00.pyz \
    pyNetwork/build/network/PYZ-00.toc \
    pyNetwork/build/network/Tree-00.toc \
    pyNetwork/build/network/Tree-01.toc \
    pyNetwork/build/network/Tree-02.toc \
    pyNetwork/build/network/base_library.zip \
    pyNetwork/build/network/localpycs/pyimod01_archive.pyc \
    pyNetwork/build/network/localpycs/pyimod02_importers.pyc \
    pyNetwork/build/network/localpycs/pyimod03_ctypes.pyc \
    pyNetwork/build/network/localpycs/pyimod04_pywin32.pyc \
    pyNetwork/build/network/localpycs/struct.pyc \
    pyNetwork/build/network/network.exe.manifest \
    pyNetwork/build/network/network.pkg \
    pyNetwork/build/network/warn-network.txt \
    pyNetwork/build/network/xref-network.html \
    pyNetwork/dist/network.exe \
    pyNetwork/ex1.csv \
    pyNetwork/main.py \
    pyNetwork/network.py \
    pyNetwork/network.spec \
    python.bat
